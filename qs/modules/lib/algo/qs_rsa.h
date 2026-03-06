/**
* @file
* @author  Huang Wen <Email:ww1383@163.com, QQ:214890094, WeChat:w18926268887>
* @version 1.0
*
* @section LICENSE
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* @section DESCRIPTION
*
* This file is ...
*/
#ifndef _QS_RSA_H_
#define _QS_RSA_H_

#ifdef __cplusplus
extern "C" {
#endif

/* RSA key lengths. */

#define MIN_RSA_MODULUS_BITS 508
/* 
   PGP 2.6.2 Now allows 2048-bit keys changing below will allow this. 
   It does lengthen key generation slightly if the value is increased. 
*/
#define MAX_RSA_MODULUS_BITS 960
//#define MAX_RSA_MODULUS_BITS 512
#define MAX_RSA_MODULUS_LEN ((MAX_RSA_MODULUS_BITS + 7) / 8)
#define MAX_RSA_PRIME_BITS ((MAX_RSA_MODULUS_BITS + 1) / 2)
#define MAX_RSA_PRIME_LEN ((MAX_RSA_PRIME_BITS + 7) / 8)


/* RSA public and private key. */
typedef struct {
	uint32_t bits;                                 /* length in bits of modulus */
	uint8_t  modulus[MAX_RSA_MODULUS_LEN];         /* modulus */
	uint8_t  exponent[MAX_RSA_MODULUS_LEN];        /* public exponent */
} qs_rsa_public_key;

typedef struct {
	uint32_t bits;                                 /* length in bits of modulus */
	uint8_t  modulus[MAX_RSA_MODULUS_LEN];         /* modulus */
	uint8_t  publicExponent[MAX_RSA_MODULUS_LEN];  /* public exponent */
	uint8_t  exponent[MAX_RSA_MODULUS_LEN];        /* private exponent */
	uint8_t  prime[2][MAX_RSA_PRIME_LEN];          /* prime factors */
	uint8_t  primeExponent[2][MAX_RSA_PRIME_LEN];  /* exponents for CRT */
	uint8_t  coefficient[MAX_RSA_PRIME_LEN];       /* CRT coefficient */
} qs_rsa_private_key;

/* RSA prototype key. */
typedef struct {
	uint32_t bits;                                 /* length in bits of modulus */
	uint32_t useFermat4;                                /* public exponent (1 = F4, 0 = 3) */
} qs_rsa_proto_key;


BOOL
qs_rsa_key_generate(
	qs_rsa_public_key  *publicKey,     /* new RSA public key */
	qs_rsa_private_key *privateKey,    /* new RSA private key */
	qs_rsa_proto_key   *protoKey       /* RSA prototype key */
);

/* RSA encryption, according to RSADSI's PKCS #1. */

BOOL 
qs_rsa_public_encrypt(
	uint8_t            *output,          /* output block */
	uint32_t           *outputLen,       /* length of output block */
	uint8_t            *input,           /* input block */
	uint32_t            inputLen,        /* length of input block */
	qs_rsa_public_key *publicKey        /* RSA public key */
);

/* RSA decryption, according to RSADSI's PKCS #1. */

BOOL
qs_rsa_public_decrypt(
	uint8_t            *output,          /* output block */
	uint32_t           *outputLen,       /* length of output block */
	uint8_t            *input,           /* input block */
	uint32_t            inputLen,        /* length of input block */
	qs_rsa_public_key *publicKey        /* RSA public key */
);

/* RSA encryption, according to RSADSI's PKCS #1. */

BOOL 
qs_rsa_private_encrypt(
	uint8_t             *output,          /* output block */
	uint32_t            *outputLen,       /* length of output block */
	uint8_t             *input,           /* input block */
	uint32_t             inputLen,        /* length of input block */
	qs_rsa_private_key *privateKey       /* RSA private key */
);

/* RSA decryption, according to RSADSI's PKCS #1. */

BOOL 
qs_rsa_private_decrypt(
	uint8_t             *output,          /* output block */
	uint32_t            *outputLen,       /* length of output block */
	uint8_t             *input,           /* input block */
	uint32_t             inputLen,        /* length of input block */
	qs_rsa_private_key *privateKey       /* RSA private key */
);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_RSA_H_
