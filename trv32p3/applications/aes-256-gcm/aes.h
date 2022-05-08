/*
 * AES functions
 * Copyright (c) 2003-2006, Jouni Malinen <j@w1.fi>
 * This software may be distributed under the terms of the BSD license.
 */

/*---------------------------------------------------------------------*/
/*                                                                     */
/*              IIII       CCCCCCCCCC       EEEEEEEEEEEE               */
/*              IIII    CCCCCCCCCCCC     EEEEEEEEEEEEEEE               */
/*              IIII   CCCCC            EEEEE                          */
/*              IIII  CCCC             EEEEE                           */
/*              IIII  CCCC             EEEE   EEEEEEEEE                */
/*              IIII  CCCC             EEEE   EEEEEEEEE                */
/*              IIII  CCCC             EEEE   EEEEEEEEE                */
/*              IIII  CCCCC            EEEEE                           */
/*              IIII   CCCCCC           EEEEEE                         */
/*              IIII    CCCCCCCCCCCCCC   EEEEEEEEEEEEEEE               */
/*              IIII       CCCCCCCCCCC      EEEEEEEEEEEE               */
/*                                                                     */
/*              INSTITUTE FOR COMMUNICATION TECHNOLOGIES               */
/*                      AND EMBEDDED SYSTEMS                           */
/*                          RWTH AACHEN                                */
/*                            GERMANY                                  */
/***********************************************************************/
/*      Any questions or comments? Contact Lennart Reimann             */
/*      E-Mail: lennart.reimann@ice.rwth-aachen.de                     */
/***********************************************************************/



#ifndef AES_H
#define AES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// typedefs for shorter data types
typedef uint8_t  u8;
typedef uint32_t u32;
typedef uint64_t u64;

// Function declarations
void * aes_encrypt_init(const u8 *key);
void aes_encrypt(void *ctx, const u8 *plain, u8 *crypt);
void aes_encrypt_deinit(void *ctx);

int rijndaelKeySetupEnc(u32 rk[], const u8 cipherKey[]);


int  aes_gcm_ae(const u8 *key,
			    const u8 *iv, size_t iv_len,
			    const u8 *plain, size_t plain_len,
			    const u8 *aad, size_t aad_len,
			    u8 *crypt, u8 *tag);

			    

// Important preprocessor defines 

#define AES_BLOCK_SIZE 16 // 128-bit or 16 byte
#define AES_PRIV_SIZE (4 * 4 * 15 + 4) // Stores 14 128-bit roundkey plus an additional one for the last AddRoundKey; and stores and number of rounds ( the last +4 in the eq.)
#define AES_PRIV_NR_POS (4 * 15) // Position in 32-bit array (rk, round keys) where number of rounds is stored

#ifndef BIT
#define BIT(x) (1 << (x))
#endif


#define RCON(i) (rcons[(i)] << 24)

static inline u32 rotr(u32 val, int bits)
{
	return (val >> bits) | (val << (32 - bits));
}

#define TE0(i) Te0[((i) >> 24) & 0xff]
#define TE1(i) rotr(Te0[((i) >> 16) & 0xff], 8)
#define TE2(i) rotr(Te0[((i) >> 8) & 0xff], 16)
#define TE3(i) rotr(Te0[(i) & 0xff], 24)
#define TE41(i) ((Te0[((i) >> 24) & 0xff] << 8) & 0xff000000)
#define TE42(i) (Te0[((i) >> 16) & 0xff] & 0x00ff0000)
#define TE43(i) (Te0[((i) >> 8) & 0xff] & 0x0000ff00)
#define TE44(i) ((Te0[(i) & 0xff] >> 8) & 0x000000ff)
#define TE421(i) ((Te0[((i) >> 16) & 0xff] << 8) & 0xff000000)
#define TE432(i) (Te0[((i) >> 8) & 0xff] & 0x00ff0000)
#define TE443(i) (Te0[(i) & 0xff] & 0x0000ff00)
#define TE414(i) ((Te0[((i) >> 24) & 0xff] >> 8) & 0x000000ff)
#define TE411(i) ((Te0[((i) >> 24) & 0xff] << 8) & 0xff000000)
#define TE422(i) (Te0[((i) >> 16) & 0xff] & 0x00ff0000)
#define TE433(i) (Te0[((i) >> 8) & 0xff] & 0x0000ff00)
#define TE444(i) ((Te0[(i) & 0xff] >> 8) & 0x000000ff)



#define GETU32(pt) (((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^ \
((u32)(pt)[2] <<  8) ^ ((u32)(pt)[3]))

#define PUTU32(ct, st) { \
(ct)[0] = (u8)((st) >> 24); (ct)[1] = (u8)((st) >> 16); \
(ct)[2] = (u8)((st) >>  8); (ct)[3] = (u8)(st); }


#define PUTU64(ct, st) 				\
		(ct)[0] = (u8) (((u64) (st)) >> 56);	\
		(ct)[1] = (u8) (((u64) (st)) >> 48);	\
		(ct)[2] = (u8) (((u64) (st)) >> 40);	\
		(ct)[3] = (u8) (((u64) (st)) >> 32);	\
		(ct)[4] = (u8) (((u64) (st)) >> 24);	\
		(ct)[5] = (u8) (((u64) (st)) >> 16);	\
		(ct)[6] = (u8) (((u64) (st)) >> 8);	\
		(ct)[7] = (u8) (((u64) (st)) & 0xff);	\
	
	






#endif /* AES_H */
