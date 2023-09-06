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

//const int16_t b_test[512] = {900, -502, -633, -295, 776, -748, -1495, -1317, -788, -1428, -1333, 80, 674, -918, 802, 452, 806, 396, 1329, 1484, -310, -881, 1613, 1169, -686, 751, 1599, 1278, 1134, 594, -767, -323, -185, 460, 554, -204, 1224, 862, 393, 787, 1132, 1507, 553, 1645, 770, 560, -948, -394, 654, -1219, 697, 557, -509, -239, 1333, 1067, 536, 1060, 998, -10, 340, 21, 939, 843, -1177, -104, -296, 259, -1198, -777, 526, 326, 1406, 1312, 1438, 1039, 1606, 278, 1563, 978, 760, 653, -411, -434, 1429, 1327, 264, 93, -1503, 502, 252, -142, 243, 1592, 927, 926, 1230, -965, 695, -467, 1112, 971, -66, 280, -354, -1433, 42, -639, 233, -1065, 1065, -140, -1013, -1538, 891, -1130, 248, 1491, 885, 13, 545, 1313, -276, 862, 263, 62, -59, -634, 823, 1469, -1663, 12, 1300, 535, -1459, -1584, 856, -1623, 610, 304, 1303, 344, 594, -1355, -1652, 5, -1289, -396, -1163, 384, -549, 1651, 161, -1008, 1469, 151, 959, -498, -1408, -1276, 1096, 381, -240, 614, -890, 304, -739, -81, -751, 1418, -225, 837, -1114, 493, 897, -1448, 398, -334, -1484, 1213, -1373, -715, -1193, 80, -1283, -1420, -1641, -862, -1483, 57, 1435, -1498, 1664, -419, 724, 602, 906, -323, -768, 511, -1350, 504, -1586, -359, -1451, 35, -1239, -49, 134, -1277, -216, 595, -601, -1312, 686, 1045, -1094, 150, -1283, -1324, -379, 883, -1295, -964, -724, 362, 1032, 1075, 388, -916, 1164, -331, 834, -867, 514, -297, 1345, -787, 120, -425, -836, 1279, 691, -1427, -1166, -725, 1019, 620, -159, -249, -807, 1536, 860, -942, -1175, -1535, -624, -114, 1636, -893, -341, -295, 222, -884, 778, 1486, -482, -289, 564, 121, -457, -1448, 151, 1480, -388, -115, -320, 965, 977, -399, 167, 1484, 1483, 144, 415, -1291, -1307, 60, -1018, 496, 1143, 529, -1610, -589, 344, 1297, 1603, 585, -1252, 1159, 680, -1360, -517, -1439, -1561, -1609, -407, -546, 1596, -58, -1099, 206, 1379, 52, -1439, 1230, -1233, -1595, 737, -877, -774, 459, 1138, -1263, 1258, -406, -689, 563, -199, 35, 507, -956, 312, -490, -255, 1277, -1326, -1129, 1444, 1093, -1500, 1476, 746, 1115, -1076, -336, 652, 1456, 1198, 1294, 1020, -81, 284, -556, 832, 16, -1327, 265, -625, -1007, 1270, 636, -782, -620, 22, -1136, 1356, -964, -485, 1498, 453, -192, 1158, -987, 1570, 1432, -25, -625, -1301, -993, 1331, 923, -143, 81, -245, 856, -822, -510, -461, -1141, -1200, 1262, -1001, -1256, 569, 1299, -946, -582, 1644, -225, 801, -8, 40, 1150, -1606, -1466, -697, -524, 163, 361, 74, 1381, 1207, -574, 1524, 46, 1300, 132, -802, 906, 55, 985, 651, 1103, -888, -1405, 1573, 250, 1516, 1212, -460, -1430, -57, -1091, -1531, 1610, 612, -948, 885, 228, -322, -1004, 865, -398, -689, 1099, -1612, -1032, -694, -655, -166, 1015, 1266, -547, -72, 425, -600, -559, 667, 1237, 1522, 1362, -610, 936, -937, -1096, -1487, -773, 983, -1596, 1648, 199, 714, -886, 21, 1590, -601, 934, 1606, -306, 1454, -904, 647, -538, -119, 221, 890, -1179, 567, 1664, -1177, 302, 180, 1544, -419, -122, 1662, -1234, -1401, -1522, -1447, 1139, -260, 1383, 1194, 848, -83, 1474, 972, -1571, 435, -1452};
//const int16_t a_test[1024] = {2443, 119, 353, 1316, 2214, 14, 2212, 2570, 297, 2322, 3273, 2833, 1234, 2321, 2852, 590, 111, 311, 1946, 2651, 2456, 1680, 443, 1993, 228, 2224, 150, 2086, 1105, 3039, 1209, 1911, 1435, 278, 2056, 3260, 2713, 259, 1643, 1478, 3088, 1828, 1543, 2777, 546, 1985, 2098, 903, 2909, 2325, 96, 333, 1922, 2574, 3023, 2279, 1618, 262, 1696, 3282, 1034, 679, 1030, 672, 2410, 1066, 677, 2224, 444, 760, 244, 425, 2266, 3292, 259, 1236, 2955, 1229, 1549, 321, 1965, 2316, 1595, 1975, 1001, 2494, 3044, 739, 788, 2823, 559, 2495, 62, 959, 325, 2757, 1787, 2856, 2871, 179, 317, 57, 3145, 529, 1069, 997, 2812, 2466, 1415, 2849, 1923, 439, 1252, 1901, 1865, 1780, 931, 769, 1385, 2011, 3291, 2994, 2107, 432, 2173, 1406, 102, 222, 615, 676, 688, 2086, 2290, 2571, 359, 3095, 832, 1677, 3276, 3249, 285, 445, 2083, 3236, 2890, 1411, 2842, 2093, 2740, 1659, 2913, 1430, 1101, 2553, 263, 2920, 300, 599, 909, 229, 1157, 612, 2489, 3077, 864, 601, 3235, 2199, 3044, 2593, 2422, 1967, 1179, 913, 858, 3305, 2438, 3313, 1887, 171, 996, 2753, 1760, 3220, 1704, 2203, 2549, 803, 40, 1413, 1880, 661, 3210, 657, 2312, 2281, 2430, 2073, 1886, 7, 1317, 1984, 2179, 2851, 1643, 873, 2018, 1777, 1710, 2624, 927, 1572, 2413, 2406, 538, 2298, 131, 550, 538, 2481, 508, 582, 2786, 87, 2102, 852, 2212, 1906, 2862, 1495, 473, 1170, 2418, 1284, 428, 89, 3262, 1725, 1430, 603, 1664, 3158, 940, 2572, 885, 3063, 621, 1045, 2745, 1189, 520, 2890, 1572, 2508, 2325, 3156, 2875, 1933, 565, 1238, 748, 325, 2556, 2234, 2440, 147, 2199, 2745, 2264, 2513, 1440, 3187, 2864, 706, 1022, 2339, 3043, 1177, 1604, 1969, 2838, 1440, 3196, 2908, 170, 943, 2574, 1464, 1408, 712, 33, 440, 2406, 980, 1124, 1734, 736, 1280, 918, 2083, 2826, 2050, 141, 637, 73, 1942, 1424, 2636, 2612, 800, 659, 1930, 588, 2231, 1264, 717, 2258, 2345, 1691, 918, 339, 1083, 512, 721, 2932, 968, 1777, 2080, 364, 2432, 235, 983, 2886, 2103, 1457, 1246, 1144, 3302, 544, 2857, 2437, 2335, 143, 2507, 908, 1278, 2826, 567, 163, 3282, 38, 1413, 3246, 1776, 1586, 1032, 42, 3228, 318, 1152, 737, 2791, 2292, 3043, 407, 694, 1196, 1055, 1637, 11, 1190, 182, 2627, 999, 1389, 1442, 1621, 1830, 184, 1544, 2667, 2328, 2152, 1387, 1935, 3323, 2729, 1969, 2430, 1430, 595, 1153, 2349, 3152, 1027, 2647, 2248, 1710, 2258, 71, 1710, 1263, 2073, 1371, 1460, 2778, 474, 2206, 1776, 2326, 1333, 2138, 1759, 3082, 271, 838, 2713, 2903, 741, 1950, 17, 1997, 1480, 645, 1577, 1610, 2734, 1152, 1433, 2890, 3187, 2017, 98, 910, 603, 282, 2115, 2462, 2117, 129, 2681, 2935, 450, 1099, 2888, 2785, 2178, 2442, 328, 700, 541, 1367, 2708, 2152, 1901, 2804, 62, 1275, 854, 2750, 1758, 298, 580, 609, 1295, 668, 3189, 2413, 2823, 2856, 1424, 2105, 725, 87, 336, 2898, 2976, 1580, 2870, 872, 249, 1420, 305, 2677, 714, 2535, 3000, 1624, 3155, 1094, 2488, 509, 994, 2249, 1870, 1520, 1343, 661, 453, 2497, 2619, 2698, 3291, 615, 3000, 1173, 2343, 3300, 1913, 2687, 2932, 1206, 1918, 2492, 620, 2371, 597, 1895, 3150, 3223, 3168, 3271, 1191, 572, 3034, 569, 1436, 19, 1662, 1350, 3094, 219, 1499, 1462, 236, 2087, 2338, 1490, 2293, 673, 2195, 2290, 443, 1645, 439, 2005, 517, 2222, 3207, 1110, 2454, 156, 3079, 1557, 599, 2803, 1671, 906, 879, 2396, 2836, 2778, 2512, 3110, 798, 2833, 329, 190, 297, 2699, 1037, 2616, 114, 902, 3004, 132, 2673, 809, 1596, 1143, 134, 3266, 1624, 3211, 1060, 464, 2476, 3226, 1302, 1951, 1607, 2924, 723, 2264, 748, 333, 2133, 1278, 1866, 1349, 1373, 1676, 522, 1112, 453, 542, 230, 2797, 2546, 3, 2010, 3077, 2732, 2298, 23, 497, 938, 2341, 2758, 2089, 804, 2736, 557, 107, 2627, 218, 1096, 3198, 2153, 113, 2829, 2048, 3320, 3164, 2409, 2637, 104, 2443, 2437, 628, 1678, 2569, 2285, 3202, 2499, 1641, 2925, 1972, 2942, 1760, 2675, 1116, 3276, 2699, 1039, 145, 1669, 1216, 1676, 2284, 1520, 3271, 745, 1785, 2253, 2076, 608, 1465, 2716, 2849, 303, 49, 3016, 1515, 1511, 53, 807, 1030, 1321, 2694, 2978, 875, 1928, 1287, 687, 2992, 2521, 1395, 2420, 1175, 2028, 3229, 3298, 2715, 537, 856, 2434, 1849, 131, 2911, 807, 360, 1601, 959, 1854, 2465, 2251, 90, 652, 801, 21, 593, 1956, 1131, 2820, 1717, 651, 1463, 874, 557, 251, 2916, 2663, 1943, 545, 48, 2610, 2054, 957, 3198, 2578, 281, 577, 123, 1887, 2772, 41, 430, 3069, 1068, 2322, 1050, 3041, 818, 917, 523, 2069, 1762, 2521, 2748, 2186, 2033, 28, 578, 2923, 1385, 2069, 2015, 2616, 535, 2442, 608, 2896, 2322, 2972, 969, 560, 1268, 588, 985, 1111, 1899, 1437, 264, 2896, 2890, 785, 1073, 1373, 2922, 475, 2972, 2936, 1331, 2837, 2895, 2158, 1897, 1017, 1798, 2851, 3265, 1315, 1653, 1055, 3128, 1244, 2614, 2404, 3008, 2904, 392, 582, 2426, 286, 511, 0, 3091, 286, 2051, 288, 1192, 3021, 1413, 2371, 1970, 2144, 2121, 2795, 1819, 1077, 2656, 362, 3183, 722, 809, 100, 1047, 2967, 948, 2204, 2797, 139, 1868, 2676, 1497, 2951, 1501, 1606, 2640, 2820, 1591, 2324, 3182, 3293, 1161, 1983, 1140, 956, 1969, 2338, 345, 3095, 2358, 704, 2688, 1383, 885, 2693, 598, 2166, 627, 745, 1017, 269, 905, 1309, 3123, 1834, 2938, 1032, 877, 2155, 2713, 2135, 2549, 1953, 3180, 550, 1870, 2043, 2862, 191, 2454, 2150, 1138, 3236, 1904, 2663, 2721, 597, 1335, 1271, 66, 3321, 1749, 3088, 67, 615, 989, 3299, 1142, 1600, 316, 1742, 1123, 2313, 2801, 88, 3134, 2529, 1720, 3139, 1490, 442, 1641, 2147, 331, 778, 411, 2395, 2083, 1109, 2793, 2114, 525, 2554, 2424, 1982, 2308, 2589, 2323, 1383, 1894, 702, 1359, 2503, 1402, 2274, 3033, 3325, 167, 711, 2360, 2054, 1379, 296, 1742, 2057, 0, 2186, 753, 2385, 2804, 225, 419, 3221, 2800, 1654, 1744, 702, 2742, 1023, 2281, 1427, 3243, 1427, 809, 367, 3208, 275, 73, 1427, 835, 1677, 498, 2746, 2085, 1548, 1500, 221, 1272, 1260, 2181, 2984, 47, 1295, 2466, 2974, 809, 2589, 1734, 2136, 2279, 566, 1270, 1693, 1742, 2571, 3126, 2696, 796, 19, 40, 606, 2871, 1712, 1544, 2336, 568, 67, 2367, 1244, 30, 3129, 495, 3066, 1990, 1769, 3100, 2708, 1039, 2956, 668};

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
