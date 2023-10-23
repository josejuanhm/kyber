///////////////////////////////////////////////////////////////////////////////////////
// October 22, 2023 - Modified by José Juan Hernández-Morales
//   Adding testing and execution time profiling instructions for RISC-V instruction 
//   set extension, as well as removing the call of now unnecessary reduction functions. 
//   Repository: https://github.com/josejuanhm/fpau
///////////////////////////////////////////////////////////////////////////////////////

#include <stddef.h>
#include <stdint.h>
#include "params.h"
#include "indcpa.h"
#include "polyvec.h"
#include "poly.h"
#include "ntt.h"
#include "symmetric.h"
#include "randombytes.h"

#include "fpau_switches.h"

#if defined(NTT_TESTING_HW) || defined(MAC_TESTING_HW) || defined(INTT_TESTING_HW) || defined(SIGNATURE_TESTING_HW) || defined(PROFILING_MAC) || defined(PROFILING_ADD_SUB)
#include "uart.h"
#endif

/*************************************************
* Name:        pack_pk
*
* Description: Serialize the public key as concatenation of the
*              serialized vector of polynomials pk
*              and the public seed used to generate the matrix A.
*
* Arguments:   uint8_t *r: pointer to the output serialized public key
*              polyvec *pk: pointer to the input public-key polyvec
*              const uint8_t *seed: pointer to the input public seed
**************************************************/
static void pack_pk(uint8_t r[KYBER_INDCPA_PUBLICKEYBYTES],
                    polyvec *pk,
                    const uint8_t seed[KYBER_SYMBYTES])
{
  size_t i;
  polyvec_tobytes(r, pk);
  for(i=0;i<KYBER_SYMBYTES;i++)
    r[i+KYBER_POLYVECBYTES] = seed[i];
}

/*************************************************
* Name:        unpack_pk
*
* Description: De-serialize public key from a byte array;
*              approximate inverse of pack_pk
*
* Arguments:   - polyvec *pk: pointer to output public-key polynomial vector
*              - uint8_t *seed: pointer to output seed to generate matrix A
*              - const uint8_t *packedpk: pointer to input serialized public key
**************************************************/
static void unpack_pk(polyvec *pk,
                      uint8_t seed[KYBER_SYMBYTES],
                      const uint8_t packedpk[KYBER_INDCPA_PUBLICKEYBYTES])
{
  size_t i;
  polyvec_frombytes(pk, packedpk);
  for(i=0;i<KYBER_SYMBYTES;i++)
    seed[i] = packedpk[i+KYBER_POLYVECBYTES];
}

/*************************************************
* Name:        pack_sk
*
* Description: Serialize the secret key
*
* Arguments:   - uint8_t *r: pointer to output serialized secret key
*              - polyvec *sk: pointer to input vector of polynomials (secret key)
**************************************************/
static void pack_sk(uint8_t r[KYBER_INDCPA_SECRETKEYBYTES], polyvec *sk)
{
  polyvec_tobytes(r, sk);
}

/*************************************************
* Name:        unpack_sk
*
* Description: De-serialize the secret key; inverse of pack_sk
*
* Arguments:   - polyvec *sk: pointer to output vector of polynomials (secret key)
*              - const uint8_t *packedsk: pointer to input serialized secret key
**************************************************/
static void unpack_sk(polyvec *sk, const uint8_t packedsk[KYBER_INDCPA_SECRETKEYBYTES])
{
  polyvec_frombytes(sk, packedsk);
}

/*************************************************
* Name:        pack_ciphertext
*
* Description: Serialize the ciphertext as concatenation of the
*              compressed and serialized vector of polynomials b
*              and the compressed and serialized polynomial v
*
* Arguments:   uint8_t *r: pointer to the output serialized ciphertext
*              poly *pk: pointer to the input vector of polynomials b
*              poly *v: pointer to the input polynomial v
**************************************************/
static void pack_ciphertext(uint8_t r[KYBER_INDCPA_BYTES], polyvec *b, poly *v)
{
  polyvec_compress(r, b);
  poly_compress(r+KYBER_POLYVECCOMPRESSEDBYTES, v);
}

/*************************************************
* Name:        unpack_ciphertext
*
* Description: De-serialize and decompress ciphertext from a byte array;
*              approximate inverse of pack_ciphertext
*
* Arguments:   - polyvec *b: pointer to the output vector of polynomials b
*              - poly *v: pointer to the output polynomial v
*              - const uint8_t *c: pointer to the input serialized ciphertext
**************************************************/
static void unpack_ciphertext(polyvec *b, poly *v, const uint8_t c[KYBER_INDCPA_BYTES])
{
  polyvec_decompress(b, c);
  poly_decompress(v, c+KYBER_POLYVECCOMPRESSEDBYTES);
}

/*************************************************
* Name:        rej_uniform
*
* Description: Run rejection sampling on uniform random bytes to generate
*              uniform random integers mod q
*
* Arguments:   - int16_t *r: pointer to output buffer
*              - unsigned int len: requested number of 16-bit integers (uniform mod q)
*              - const uint8_t *buf: pointer to input buffer (assumed to be uniformly random bytes)
*              - unsigned int buflen: length of input buffer in bytes
*
* Returns number of sampled 16-bit integers (at most len)
**************************************************/
static unsigned int rej_uniform(int16_t *r,
                                unsigned int len,
                                const uint8_t *buf,
                                unsigned int buflen)
{
  unsigned int ctr, pos;
  uint16_t val0, val1;

  ctr = pos = 0;
  while(ctr < len && pos + 3 <= buflen) {
    val0 = ((buf[pos+0] >> 0) | ((uint16_t)buf[pos+1] << 8)) & 0xFFF;
    val1 = ((buf[pos+1] >> 4) | ((uint16_t)buf[pos+2] << 4)) & 0xFFF;
    pos += 3;

    if(val0 < KYBER_Q)
      r[ctr++] = val0;
    if(ctr < len && val1 < KYBER_Q)
      r[ctr++] = val1;
  }

  return ctr;
}

#define gen_a(A,B)  gen_matrix(A,B,0)
#define gen_at(A,B) gen_matrix(A,B,1)

/*************************************************
* Name:        gen_matrix
*
* Description: Deterministically generate matrix A (or the transpose of A)
*              from a seed. Entries of the matrix are polynomials that look
*              uniformly random. Performs rejection sampling on output of
*              a XOF
*
* Arguments:   - polyvec *a: pointer to ouptput matrix A
*              - const uint8_t *seed: pointer to input seed
*              - int transposed: boolean deciding whether A or A^T is generated
**************************************************/
#define GEN_MATRIX_NBLOCKS ((12*KYBER_N/8*(1 << 12)/KYBER_Q + XOF_BLOCKBYTES)/XOF_BLOCKBYTES)
// Not static for benchmarking
void gen_matrix(polyvec *a, const uint8_t seed[KYBER_SYMBYTES], int transposed)
{
  unsigned int ctr, i, j, k;
  unsigned int buflen, off;
  uint8_t buf[GEN_MATRIX_NBLOCKS*XOF_BLOCKBYTES+2];
  xof_state state;

  for(i=0;i<KYBER_K;i++) {
    for(j=0;j<KYBER_K;j++) {
      if(transposed)
        xof_absorb(&state, seed, i, j);
      else
        xof_absorb(&state, seed, j, i);

      xof_squeezeblocks(buf, GEN_MATRIX_NBLOCKS, &state);
      buflen = GEN_MATRIX_NBLOCKS*XOF_BLOCKBYTES;
      ctr = rej_uniform(a[i].vec[j].coeffs, KYBER_N, buf, buflen);

      while(ctr < KYBER_N) {
        off = buflen % 3;
        for(k = 0; k < off; k++)
          buf[k] = buf[buflen - off + k];
        xof_squeezeblocks(buf + off, 1, &state);
        buflen = off + XOF_BLOCKBYTES;
        ctr += rej_uniform(a[i].vec[j].coeffs + ctr, KYBER_N - ctr, buf, buflen);
      }
    }
  }
}

/*************************************************
* Name:        indcpa_keypair
*
* Description: Generates public and private key for the CPA-secure
*              public-key encryption scheme underlying Kyber
*
* Arguments:   - uint8_t *pk: pointer to output public key
*                             (of length KYBER_INDCPA_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output private key
                              (of length KYBER_INDCPA_SECRETKEYBYTES bytes)
**************************************************/
void indcpa_keypair(uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES],
                    uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES])
{
  unsigned int i;
  uint8_t buf[2*KYBER_SYMBYTES] = {0};
  const uint8_t *publicseed = buf;
  const uint8_t *noiseseed = buf+KYBER_SYMBYTES;
  uint8_t nonce = 0;
  polyvec a[KYBER_K], e, pkpv, skpv;

#ifdef PROFILING_MAC
  register uint32_t cycle_start asm("s4");
  register uint32_t cycle_end asm("s3");
#endif

  randombytes(buf, KYBER_SYMBYTES);
  hash_g(buf, buf, KYBER_SYMBYTES);

  gen_a(a, publicseed);

  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta1(&skpv.vec[i], noiseseed, nonce++);
  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta1(&e.vec[i], noiseseed, nonce++);

  polyvec_ntt(&skpv);

#ifdef NTT_TESTING_HW
  int16_t* skpv_ptr = (int16_t*)&skpv;
  uart_send_string("\n\rskpv_full\n");
  for (uint16_t i = 0; i < KYBER_N*KYBER_K; i++)
  {
    itoa(pbuf, skpv_ptr[i], 10);
    uart_send_string("\r");
    uart_send_string(str);
  }
#endif

  polyvec_ntt(&e);

  // matrix-vector multiplication
  for(i=0;i<KYBER_K;i++) {
#ifdef PROFILING_MAC
    uart_send_string("\n\rMAC polyvec_basemul");
    asm("csrrs s4, "TICKS_REGISTER", zero");
#endif
    polyvec_basemul_acc_montgomery(&pkpv.vec[i], &a[i], &skpv);
#ifdef PROFILING_MAC
    asm("csrrs s3, "TICKS_REGISTER", zero");
    print_runtime(cycle_start, cycle_end);
#endif
#ifndef FPAU
    poly_tomont(&pkpv.vec[i]);
#endif
  }

#ifdef NTT_TESTING_HW
  int16_t* pkpv_ptr = (int16_t*)&pkpv;
  uart_send_string("\n\rpkpv_pre\n");
  for (uint16_t i = 0; i < KYBER_N*KYBER_K; i++)
  {
    itoa(pbuf, pkpv_ptr[i], 10);
    uart_send_string("\r");
    uart_send_string(str);
  }
#endif

  polyvec_add(&pkpv, &pkpv, &e);
#ifndef FPAU
  polyvec_reduce(&pkpv);
#endif

#ifdef NTT_TESTING_HW
  uart_send_string("\n\rpkpv_post\n");
  for (uint16_t i = 0; i < KYBER_N*KYBER_K; i++)
  {
    itoa(pbuf, pkpv_ptr[i], 10);
    uart_send_string("\r");
    uart_send_string(str);
  }
#endif

  pack_sk(sk, &skpv);
  pack_pk(pk, &pkpv, publicseed);
}

/*************************************************
* Name:        indcpa_enc
*
* Description: Encryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - uint8_t *c: pointer to output ciphertext
*                            (of length KYBER_INDCPA_BYTES bytes)
*              - const uint8_t *m: pointer to input message
*                                  (of length KYBER_INDCPA_MSGBYTES bytes)
*              - const uint8_t *pk: pointer to input public key
*                                   (of length KYBER_INDCPA_PUBLICKEYBYTES)
*              - const uint8_t *coins: pointer to input random coins used as seed
*                                      (of length KYBER_SYMBYTES) to deterministically
*                                      generate all randomness
**************************************************/
void indcpa_enc(uint8_t c[KYBER_INDCPA_BYTES],
                const uint8_t m[KYBER_INDCPA_MSGBYTES],
                const uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES],
                const uint8_t coins[KYBER_SYMBYTES])
{
  unsigned int i;
  uint8_t seed[KYBER_SYMBYTES] = {0};
  uint8_t nonce = 0;
  polyvec sp, pkpv, ep, at[KYBER_K], b;
  poly v, k, epp;

#ifdef PROFILING_ADD_SUB
  register uint32_t cycle_start asm("s4");
  register uint32_t cycle_end asm("s3");
#endif

  unpack_pk(&pkpv, seed, pk);
  poly_frommsg(&k, m);
  gen_at(at, seed);

  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta1(sp.vec+i, coins, nonce++);
  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta2(ep.vec+i, coins, nonce++);
  poly_getnoise_eta2(&epp, coins, nonce++);

#ifdef NTT_TESTING_HW
  uart_send_string("\n\rcoins\n");
  for (uint16_t i = 0; i < KYBER_SYMBYTES; i++)
  {
    itoa(pbuf, coins[i], 10);
    uart_send_string("\r");
    uart_send_string(str);
  }

  int16_t* sp_ptr = (int16_t*)&sp;
  uart_send_string("\n\rsp_in\n");
  for (uint16_t i = 0; i < KYBER_N*KYBER_K; i++)
  {
    itoa(pbuf, sp_ptr[i], 10);
    uart_send_string("\r");
    uart_send_string(str);
  }
#endif

  polyvec_ntt(&sp);

#ifdef NTT_TESTING_HW
  uart_send_string("\n\rsp_full\n");
  for (uint16_t i = 0; i < KYBER_N*KYBER_K; i++)
  {
    itoa(pbuf, sp_ptr[i], 10);
    uart_send_string("\r");
    uart_send_string(str);
  }
#endif

  // matrix-vector multiplication
  for(i=0;i<KYBER_K;i++)
    polyvec_basemul_acc_montgomery(&b.vec[i], &at[i], &sp);

  polyvec_basemul_acc_montgomery(&v, &pkpv, &sp);

#ifdef NTT_TESTING_HW
  uart_send_string("\n\rb_in\n");
  int16_t* b_ptr = (int16_t*)&b;
  for (uint16_t i = 0; i < KYBER_N*KYBER_K; i++)
  {
    itoa(pbuf, b_ptr[i], 10);
    uart_send_string("\r");
    uart_send_string(str);
  }
#endif

  polyvec_invntt_tomont(&b);

#ifdef NTT_TESTING_HW
  uart_send_string("\n\rb_out\n");
  for (uint16_t i = 0; i < KYBER_N*KYBER_K; i++)
  {
    itoa(pbuf, b_ptr[i], 10);
    uart_send_string("\r");
    uart_send_string(str);
  }
#endif

  poly_invntt_tomont(&v);

  polyvec_add(&b, &b, &ep);
  poly_add(&v, &v, &epp);

#ifdef PROFILING_ADD_SUB
    uart_send_string("\n\rADD poly_add");
    asm("csrrs s4, "TICKS_REGISTER", zero");
#endif
  poly_add(&v, &v, &k);
#ifndef FPAU
  polyvec_reduce(&b);
  poly_reduce(&v);
#endif
#ifdef PROFILING_ADD_SUB
    asm("csrrs s3, "TICKS_REGISTER", zero");
    print_runtime(cycle_start, cycle_end);
#endif

#ifndef FPAU
  polyvec_reduce(&b);
#endif

#ifdef NTT_TESTING_HW
  uart_send_string("\n\rb_reduced\n");
  for (uint16_t i = 0; i < KYBER_N*KYBER_K; i++)
  {
    itoa(pbuf, b_ptr[i], 10);
    uart_send_string("\r");
    uart_send_string(str);
  }
#endif

  pack_ciphertext(c, &b, &v);
}

/*************************************************
* Name:        indcpa_dec
*
* Description: Decryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - uint8_t *m: pointer to output decrypted message
*                            (of length KYBER_INDCPA_MSGBYTES)
*              - const uint8_t *c: pointer to input ciphertext
*                                  (of length KYBER_INDCPA_BYTES)
*              - const uint8_t *sk: pointer to input secret key
*                                   (of length KYBER_INDCPA_SECRETKEYBYTES)
**************************************************/
void indcpa_dec(uint8_t m[KYBER_INDCPA_MSGBYTES],
                const uint8_t c[KYBER_INDCPA_BYTES],
                const uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES])
{
  polyvec b, skpv;
  poly v, mp;

#ifdef PROFILING_ADD_SUB
  register uint32_t cycle_start asm("s4");
  register uint32_t cycle_end asm("s3");
#endif

  unpack_ciphertext(&b, &v, c);
  unpack_sk(&skpv, sk);

  polyvec_ntt(&b);

#ifdef NTT_TESTING_HW
  int16_t* b_ptr = (int16_t*)&b;
  uart_send_string("\n\rb_full_decodeStage\n");
  for (uint16_t i = 0; i < KYBER_N*KYBER_K; i++)
  {
    itoa(pbuf, b_ptr[i], 10);
    uart_send_string("\r");
    uart_send_string(str);
  }
#endif

  polyvec_basemul_acc_montgomery(&mp, &skpv, &b);
  poly_invntt_tomont(&mp);

#ifdef NTT_TESTING_HW
  int16_t* mp_ptr = (int16_t*)&mp;
  uart_send_string("\n\rmp\n");
  for (uint16_t i = 0; i < KYBER_N; i++)
  {
    itoa(pbuf, mp_ptr[i], 10);
    uart_send_string("\r");
    uart_send_string(str);
  }
#endif

#ifdef PROFILING_ADD_SUB
    uart_send_string("\n\rSUB poly_sub");
    asm("csrrs s4, "TICKS_REGISTER", zero");
#endif
  poly_sub(&mp, &v, &mp);

#ifndef FPAU
  poly_reduce(&mp);
#endif

#ifdef PROFILING_ADD_SUB
    asm("csrrs s3, "TICKS_REGISTER", zero");
    print_runtime(cycle_start, cycle_end);
#endif

  poly_tomsg(m, &mp);
}
