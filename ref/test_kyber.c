///////////////////////////////////////////////////////////////////////////////////////
// October 22, 2023 - Modified by José Juan Hernández-Morales
//   Adding testing and execution time profiling instructions for RISC-V instruction 
//   set extension. 
//   Repository: https://github.com/josejuanhm/fpau
///////////////////////////////////////////////////////////////////////////////////////

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "kem.h"
#include "randombytes.h"

#include "fpau_switches.h"

#define NTESTS 10000

#ifdef UART
#include "uart.h"
#endif

static int test_keys(unsigned int i)
{
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t key_a[CRYPTO_BYTES];
  uint8_t key_b[CRYPTO_BYTES];

#ifdef PROFILING_STAGES
  register uint32_t cycle_start  asm("s2");
  register uint32_t cycle_end    asm("s3");
  register uint32_t cycle_start2 asm("s4");
  register uint32_t cycle_end2   asm("s5");
  register uint32_t cycle_start3 asm("s6");
  register uint32_t cycle_end3   asm("s7");

  static float keys_avg = 0;
  static float enc_avg  = 0;
  static float dec_avg  = 0;
#endif

#ifdef UART
  uart_send_string("\n\rCreate initial keys");
#endif

#ifdef PROFILING_STAGES
  asm("csrrs s2, "TICKS_REGISTER", zero");
#endif
  //Alice generates a public key
  crypto_kem_keypair(pk, sk);
#ifdef PROFILING_STAGES
  asm("csrrs s3, "TICKS_REGISTER", zero");
  print_runtime(cycle_start, cycle_end);
  keys_avg = ((cycle_end - cycle_start) + i*keys_avg) / (i+1); // obtaining average per new sample
#endif

#ifdef UART
  uart_send_string("\nEncode, create key b");
#endif

#ifdef PROFILING_STAGES
  asm("csrrs s4, "TICKS_REGISTER", zero");
#endif
  //Bob derives a secret key and creates a response
  crypto_kem_enc(ct, key_b, pk);
#ifdef PROFILING_STAGES
  asm("csrrs s5, "TICKS_REGISTER", zero");
  print_runtime(cycle_start2, cycle_end2);
  enc_avg = ((cycle_end2 - cycle_start2) + i*enc_avg) / (i+1);
#endif

#ifdef UART
  uart_send_string("\nDecode, create key a");
#endif

#ifdef PROFILING_STAGES
  asm("csrrs s6, "TICKS_REGISTER", zero");
#endif
  //Alice uses Bobs response to get her shared key
  crypto_kem_dec(key_a, ct, sk);
#ifdef PROFILING_STAGES
  asm("csrrs s7, "TICKS_REGISTER", zero");
  print_runtime(cycle_start3, cycle_end3);
  dec_avg = ((cycle_end3 - cycle_start3) + i*dec_avg) / (i+1);
#endif

  if(memcmp(key_a, key_b, CRYPTO_BYTES)) {
#ifndef FPAU
    printf("ERROR keys\n");
#endif
#ifdef UART
    uart_send_string("\n\rERROR keys");
#endif
    return 1;
  }

#ifdef PROFILING_STAGES
  // print average runtimes
  if (i == NTESTS - 1)
  {
    itoa(pbuf, (unsigned int)keys_avg, 10);
    uart_send_string("\n\rKeys average: ");
    uart_send_string(str);

    itoa(pbuf, (unsigned int)enc_avg, 10);
    uart_send_string("\nEncode average: ");
    uart_send_string(str);

    itoa(pbuf, (unsigned int)dec_avg, 10);
    uart_send_string("\nDecode average: ");
    uart_send_string(str);
  }
#endif

  return 0;
}

static int test_invalid_sk_a()
{
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t key_a[CRYPTO_BYTES];
  uint8_t key_b[CRYPTO_BYTES];

  //Alice generates a public key
  crypto_kem_keypair(pk, sk);

  //Bob derives a secret key and creates a response
  crypto_kem_enc(ct, key_b, pk);

  //Replace secret key with random values
  randombytes(sk, CRYPTO_SECRETKEYBYTES);

  //Alice uses Bobs response to get her shared key
  crypto_kem_dec(key_a, ct, sk);

  if(!memcmp(key_a, key_b, CRYPTO_BYTES)) {
#ifndef FPAU
    printf("ERROR invalid sk\n");
#endif
#ifdef UART
    uart_send_string("ERROR invalid sk\n");
#endif
    return 1;
  }

  return 0;
}

static int test_invalid_ciphertext()
{
  uint8_t pk[CRYPTO_PUBLICKEYBYTES];
  uint8_t sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
  uint8_t key_a[CRYPTO_BYTES];
  uint8_t key_b[CRYPTO_BYTES];
  uint8_t b;
  size_t pos;

  do {
    randombytes(&b, sizeof(uint8_t));
  } while(!b);
  randombytes((uint8_t *)&pos, sizeof(size_t));

  //Alice generates a public key
  crypto_kem_keypair(pk, sk);

  //Bob derives a secret key and creates a response
  crypto_kem_enc(ct, key_b, pk);

  //Change some byte in the ciphertext (i.e., encapsulated key)
  ct[pos % CRYPTO_CIPHERTEXTBYTES] ^= b;

  //Alice uses Bobs response to get her shared key
  crypto_kem_dec(key_a, ct, sk);

  if(!memcmp(key_a, key_b, CRYPTO_BYTES)) {
#ifndef FPAU
    prinfr("ERROR invalid ciphertext\n");
#endif
#ifdef UART
    uart_send_string("ERROR invalid ciphertext\n");
#endif
    return 1;
  }

  return 0;
}

int main(void)
{
  unsigned int i;
  int r;

  for(i=0;i<NTESTS;i++) {
    r  = test_keys(i);
    r |= test_invalid_sk_a();
    r |= test_invalid_ciphertext();
    if(r)
      return 1;
  }

/*
  printf("CRYPTO_SECRETKEYBYTES:  %d\n",CRYPTO_SECRETKEYBYTES);
  printf("CRYPTO_PUBLICKEYBYTES:  %d\n",CRYPTO_PUBLICKEYBYTES);
  printf("CRYPTO_CIPHERTEXTBYTES: %d\n",CRYPTO_CIPHERTEXTBYTES);
*/
  return 0;
}
