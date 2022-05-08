/*
 * Galois/Counter Mode (GCM) with AES
 * Copyright (c) 2012, Jouni Malinen <j@w1.fi>
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


#include "aes.h"

static void inc32(u8 *block)
{
	u32 val;
	val = GETU32(block + AES_BLOCK_SIZE - 4);
	val++;
	PUTU32(block + AES_BLOCK_SIZE - 4, val);
}


static void xor_block(u8 *dst, const u8 *src)
{
	u32 *d = (u32 *) dst;
	u32 *s = (u32 *) src;
	*d++ ^= *s++;
	*d++ ^= *s++;
	*d++ ^= *s++;
	*d++ ^= *s++;
}


static void shift_right_block(u8 *v)
{
	u32 val;

	val = GETU32(v + 12);
	val >>= 1;
	if (v[11] & 0x01)
		val |= 0x80000000;
	PUTU32(v + 12, val);

	val = GETU32(v + 8);
	val >>= 1;
	if (v[7] & 0x01)
		val |= 0x80000000;
	PUTU32(v + 8, val);

	val = GETU32(v + 4);
	val >>= 1;
	if (v[3] & 0x01)
		val |= 0x80000000;
	PUTU32(v + 4, val);

	val = GETU32(v);
	val >>= 1;
	PUTU32(v, val);
}


/* Multiplication in GF(2^128) */
static void gf_mult(const u8 *x, const u8 *y, u8 *z)
{
	u8 v[16];
	int i, j;

	memset(z, 0, 16); /* Z_0 = 0^128 */
	memcpy(v, y, 16); /* V_0 = Y */

	for (i = 0; i < 16; i++) {
		for (j = 0; j < 8; j++) {
			if (x[i] & BIT(7 - j)) {
				/* Z_(i + 1) = Z_i XOR V_i */
				xor_block(z, v);
			} else {
				/* Z_(i + 1) = Z_i */
			}

			if (v[15] & 0x01) {
				/* V_(i + 1) = (V_i >> 1) XOR R */
				shift_right_block(v);
				/* R = 11100001 || 0^120 */
				v[0] ^= 0xe1;
			} else {
				/* V_(i + 1) = V_i >> 1 */
				shift_right_block(v);
			}
		}
	}
}


static void ghash_start(u8 *y)
{
	/* Y_0 = 0^128 */
	memset(y, 0, 16);
}


static void ghash(const u8 *h, const u8 *x, size_t xlen, u8 *y)
{
	size_t m, i;
	const u8 *xpos = x;
	u8 tmp[16];

	m = xlen / 16;

	for (i = 0; i < m; i++) {
		/* Y_i = (Y^(i-1) XOR X_i) dot H */
		xor_block(y, xpos);
		xpos += 16;

		/* dot operation:
		 * multiplication operation for binary Galois (finite) field of
		 * 2^128 elements */
		gf_mult(y, h, tmp);
		memcpy(y, tmp, 16);
	}

	if (x + xlen > xpos) {
		/* Add zero padded last block */
		size_t last = x + xlen - xpos;
		memcpy(tmp, xpos, last);
		memset(tmp + last, 0, sizeof(tmp) - last);

		/* Y_i = (Y^(i-1) XOR X_i) dot H */
		xor_block(y, tmp);

		/* dot operation:
		 * multiplication operation for binary Galois (finite) field of
		 * 2^128 elements */
		gf_mult(y, h, tmp);
		memcpy(y, tmp, 16);
	}

	/* Return Y_m */
}


static void aes_gctr(void *aes, const u8 *icb, const u8 *x, size_t xlen, u8 *y)
{
	size_t i, n, last;
	u8 cb[AES_BLOCK_SIZE], tmp[AES_BLOCK_SIZE];
	const u8 *xpos = x;
	u8 *ypos = y;

	if (xlen == 0)
		return;

	n = xlen / 16; // How many 128-bit plaintext blocks are in the plaintext

	memcpy(cb, icb, AES_BLOCK_SIZE); // Copy the J0 or J1 which is used as plaintext for aes
	/* Full blocks */
	for (i = 0; i < n; i++) {
		aes_encrypt(aes, cb, ypos);
		xor_block(ypos, xpos); // Xor the actual plaintext with the encrypted current J-value
		xpos += AES_BLOCK_SIZE; // Increment the pointers
		ypos += AES_BLOCK_SIZE;
		inc32(cb); // Increment the J-value
	}

	last = x + xlen - xpos; // Check if blocks with less than 128-bits are left
	if (last) {
		/* Last, partial block */
		aes_encrypt(aes, cb, tmp);
		
		for (i = 0; i < last; i++)
			*ypos++ = *xpos++ ^ tmp[i];
	}
}


static void * aes_gcm_init_hash_subkey(const u8 *key, u8 *H)
{
	void *aes;

	aes = aes_encrypt_init(key);
	if (aes == NULL)
		return NULL;

	/* Generate hash subkey H = AES_K(0^128) */
	memset(H, 0, AES_BLOCK_SIZE);
	aes_encrypt(aes, H, H);

	return aes;
}


static void aes_gcm_prepare_j0(const u8 *iv, size_t iv_len, const u8 *H, u8 *J0)
{
	u8 len_buf[16];

	if (iv_len == 12) {
		/* Prepare block J_0 = IV || 0^31 || 1 [len(IV) = 96] */
		memcpy(J0, iv, iv_len);
		memset(J0 + iv_len, 0, AES_BLOCK_SIZE - iv_len);
		J0[AES_BLOCK_SIZE - 1] = 0x01;
	} else {
		/*
		 * s = 128 * ceil(len(IV)/128) - len(IV)
		 * J_0 = GHASH_H(IV || 0^(s+64) || [len(IV)]_64)
		 */
		ghash_start(J0);
		ghash(H, iv, iv_len, J0);
		PUTU64(len_buf, 0);
		PUTU64(len_buf + 8, iv_len * 8);
		ghash(H, len_buf, sizeof(len_buf), J0);
	}
}


static void aes_gcm_gctr(void *aes, const u8 *J0, const u8 *plain, size_t plain_len,
			 u8 *crypt)
{
	u8 J0inc[AES_BLOCK_SIZE];

	if (plain_len == 0)
		return;

	memcpy(J0inc, J0, AES_BLOCK_SIZE);
	// Increment J0 for the first plaintext
	inc32(J0inc);
	aes_gctr(aes, J0inc, plain, plain_len, crypt);
}


static void aes_gcm_ghash(const u8 *H, const u8 *aad, size_t aad_len,
			  const u8 *crypt, size_t crypt_len, u8 *S)
{
	u8 len_buf[16];

	/*
	 * u = 128 * ceil[len(C)/128] - len(C)
	 * v = 128 * ceil[len(A)/128] - len(A)
	 * S = GHASH_H(A || 0^v || C || 0^u || [len(A)]64 || [len(C)]64)
	 * (i.e., zero padded to block size A || C and lengths of each in bits)
	 */
	ghash_start(S);
	ghash(H, aad, aad_len, S);
	ghash(H, crypt, crypt_len, S);
	PUTU64(len_buf, aad_len * 8);
	PUTU64(len_buf + 8, crypt_len * 8);
	ghash(H, len_buf, sizeof(len_buf), S);
}


/**
 * aes_gcm_ae - GCM-AE_K(IV, P, A)
 */
int aes_gcm_ae(const u8 *key, const u8 *iv, size_t iv_len,
	       const u8 *plain, size_t plain_len,
	       const u8 *aad, size_t aad_len, u8 *crypt, u8 *tag)
{
	u8 H[AES_BLOCK_SIZE];
	u8 J0[AES_BLOCK_SIZE];
	u8 S[16];
	void *aes;

        // Initializing the HASH Key from the Encryption Key for the Galois Hashing
        // This is done by encrypting 128-bits equal to zero with the encryption key K and storing the ciphertext
        // as the HASH key in H
	aes = aes_gcm_init_hash_subkey(key, H);
	if (aes == NULL)
		return -1;
        // Prepare the initialization of J0:
        /* Define J0, as follows:
            If bit_len(IV)=96, then J0 =IV||0^31||1
            If bit_len(IV)≠96, then let s=128∙ceil(bit_len(IV)/128)-bit_len(IV), 
                and let J0=GHASH_H(IV||0^(s+64)||[bit_len(IV)]_64)
            */
	aes_gcm_prepare_j0(iv, iv_len, H, J0);

	/* C = GCTR_K(inc_32(J_0), P) */
	// Encryption is done in this function J0 is encrypted using AES-256
	// and XORed with the plaintext. If there is more than one BLOCK (128-bit) of plaintext
	// then a chaining method is used and additional J0 (J1, J1 by incrementing) need to be computed
	// They are then used as 'plaintext' for the aes-256 encryption and XORed with the remaining 
	// plaintexts
	aes_gcm_gctr(aes, J0, plain, plain_len, crypt);

        // In this part the additional hashing is done which is used to prove authenticity and 
        // integrity of the data transmitted. For this purpose the following parameters are needed:
        // H: Hashing key computed in aes_gcm_prepare_j0 and is 128 bits long
        // aad: Additional authenticated data is data that doesnt have to be encrypted but shall be hashed
        //      to prove authenticity to the receiver
        // crypt: is the encrypted data, which is used with the aad-data to form a hash
        // Additionally the length of the crypto is passed which is equal to the length of the plaintext. 
        // (Although the length of the output of AES is fixed it is still XORed with the plaintext and the result is
        // cut to the length of the plaintext.
	aes_gcm_ghash(H, aad, aad_len, crypt, plain_len, S);

	/* T = MSB_t(GCTR_K(J_0, S)) */
	// Encrypt the value of J0 and xor it with the hash S to form the tag
	aes_gctr(aes, J0, S, sizeof(S), tag);

	/* Return (C, T) */

	aes_encrypt_deinit(aes);

	return 0;
}


