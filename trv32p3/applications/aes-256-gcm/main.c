/*
 * Test program for AES
 * Copyright (c) 2003-2012, Jouni Malinen <j@w1.fi>
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

#define NUM_TEST_CASES 6

// Transformation function used for checking the test case results..
static int hex2num(char c)
{
        if (c >= '0' && c <= '9')
                return c - '0';
        if (c >= 'a' && c <= 'f')
                return c - 'a' + 10;
        if (c >= 'A' && c <= 'F')
                return c - 'A' + 10;
        return -1;
}

// Transformation function used for checking the test case results..

int hex2byte(const char *hex)
{
       int a, b;
       a = hex2num(*hex++);
       if (a < 0)
               return -1;
       b = hex2num(*hex++);
       if (b < 0)
               return -1;
       return (a << 4) | b;
}

int hexstr2bin(const char *hex, u8 *buf, size_t len)
{
       size_t i;
       int a;
       const char *ipos = hex;
       u8 *opos = buf;
       for (i = 0; i < len; i++) {
               a = hex2byte(ipos);
               if (a < 0)
                       return -1;
               *opos++ = a;
               ipos += 2;
       }
       return 0;
}

/*
 * GCM test vectors from
 * http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/gcm/gcm-spec.pdf
 */
struct gcm_test_vector {
	const char *k; // Key
	const char *p; // Plaintext
	const char *aad; // Additional Authenticated Data
	const char *iv; // Initialization Vector
	const char *c; // Ciphertext
	const char *t; // Tag
};
static const struct gcm_test_vector gcm_tests[] = {
	{
		/* Test Case 0 */
		"0000000000000000000000000000000000000000000000000000000000000000",
		"",
		"",
		"000000000000000000000000",
		"",
		"530f8afbc74536b9a963b4f1c4cb738b"
	},
	{
		/* Test Case 1*/
		"0000000000000000000000000000000000000000000000000000000000000000",
		"00000000000000000000000000000000",
		"",
		"000000000000000000000000",
		"cea7403d4d606b6e074ec5d3baf39d18",
		"d0d1c8a799996bf0265b98b5d48ab919"
	},
	{
		/* Test Case 2 */
		"feffe9928665731c6d6a8f9467308308feffe9928665731c6d6a8f9467308308",
		"d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255",
		"",
		"cafebabefacedbaddecaf888",
		"522dc1f099567d07f47f37a32a84427d643a8cdcbfe5c0c97598a2bd2555d1aa8cb08e48590dbb3da7b08b1056828838c5f61e6393ba7a0abcc9f662898015ad",
		"b094dac5d93471bdec1a502270e3cc6c"
	},
	{
		/* Test Case 3 */
		"feffe9928665731c6d6a8f9467308308feffe9928665731c6d6a8f9467308308",
		"d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39",
		"feedfacedeadbeeffeedfacedeadbeefabaddad2",
		"cafebabefacedbaddecaf888",
		"522dc1f099567d07f47f37a32a84427d643a8cdcbfe5c0c97598a2bd2555d1aa8cb08e48590dbb3da7b08b1056828838c5f61e6393ba7a0abcc9f662",
		"76fc6ece0f4e1768cddf8853bb2d551b"
	},
	{
		/* Test Case 4 */
		"feffe9928665731c6d6a8f9467308308feffe9928665731c6d6a8f9467308308",
		"d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39",
		"feedfacedeadbeeffeedfacedeadbeefabaddad2",
		"cafebabefacedbad",
		"c3762df1ca787d32ae47c13bf19844cbaf1ae14d0b976afac52ff7d79bba9de0feb582d33934a4f0954cc2363bc73f7862ac430e64abe499f47c9b1f",
		"3a337dbf46a792c45e454913fe2ea8f2"
	},
	{
		/* Test Case 5 */
		"feffe9928665731c6d6a8f9467308308feffe9928665731c6d6a8f9467308308",
		"d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39",
		"feedfacedeadbeeffeedfacedeadbeefabaddad2",
		"9313225df88406e555909c5aff5269aa6a7a9538534f7da1e4c303d2a318a728c3c0c95156809539fcf0e2429a6b525416aedbf5a0de6a57a637b39b",
		"5a8def2f0c9e53f1f75d7853659e2a20eeb2b22aafde6419a058ab4f6f746bf40fc0c3b780f244452da3ebf1c5d82cdea2418997200ef82e44ae7e3f",
		"a44a8266ee1c8eb0c8b5d4cf5ae9f19a"
	}
};
int main(void)
{
	int total_cycle_count = 0;
	int start_cycle_count = 0;
	int end_cycle_count = 0;
	int ret = 0;
	int i;
	u8 k[32], aad[32], iv[64], t[16], tag[16];
	u8 p[64], c[64], tmp[64];
	size_t k_len, p_len, aad_len, iv_len;
	int num_correct_test_cases = 0;
	int test_case_correct;
	for (i = 0; i < NUM_TEST_CASES; i++) {
		const struct gcm_test_vector *tc = &gcm_tests[i];
		k_len = strlen(tc->k) / 2;
		test_case_correct = 1;
		
		// Not part of the performance check!
		if (hexstr2bin(tc->k, k, k_len)) {
			printf("Invalid GCM test vector %d (k)\n", i);
			test_case_correct = 0;
			ret++;
			continue;
		}
		p_len = strlen(tc->p) / 2;
		if (hexstr2bin(tc->p, p, p_len)) {
			printf("Invalid GCM test vector %d (p)\n", i);
			test_case_correct = 0;
			ret++;
			continue;
		}
		aad_len = strlen(tc->aad) / 2;
		if (hexstr2bin(tc->aad, aad, aad_len)) {
			printf("Invalid GCM test vector %d (aad)\n", i);
			test_case_correct = 0;
			ret++;
			continue;
		}
		iv_len = strlen(tc->iv) / 2;
		if (hexstr2bin(tc->iv, iv, iv_len)) {
			printf("Invalid GCM test vector %d (iv)\n", i);
			test_case_correct = 0;
			ret++;
			continue;
		}
		if (hexstr2bin(tc->c, c, p_len)) {
			printf("Invalid GCM test vector %d (c)\n", i);
			test_case_correct = 0;
			ret++;
			continue;
		}
		if (hexstr2bin(tc->t, t, sizeof(t))) {
			printf("Invalid GCM test vector %d (t)\n", i);
			test_case_correct = 0;
			ret++;
			continue;
		}

		start_cycle_count = chess_cycle_count();
		if (aes_gcm_ae(k, iv, iv_len, p, p_len, aad, aad_len,
			       tmp, tag) < 0) {
			printf("GCM-AE failed (test case %d)\n", i);
			test_case_correct = 0;
			ret++;
			continue;
		}
		else
		{
		        printf("GCM-AE succeeded (test case %d)\n", i);
		}
		end_cycle_count = chess_cycle_count();
		total_cycle_count += end_cycle_count - start_cycle_count;
		if (memcmp(c, tmp, p_len) != 0) {
			printf("GCM-AE mismatch (test case %d)\n", i);
			test_case_correct = 0;
			ret++;
		}
		else
		{
		        printf("GCM-AE match (test case %d)\n", i);

		}
		if (memcmp(tag, t, sizeof(tag)) != 0) {
			printf("GCM-AE tag mismatch (test case %d)\n", i);
			test_case_correct = 0;
			ret++;
		}
	        else
	        {
	                printf("GCM-AE tag match (test case %d)\n", i);
		}
		num_correct_test_cases += test_case_correct;
	}
	
	printf("\n\n\n%d of %d test cases executed and computed correctly!\n",num_correct_test_cases, NUM_TEST_CASES);
	printf("Total cycle count in encryption function: %d\n", total_cycle_count);
	printf("Cycle count per test case in encryption function: %d\n", total_cycle_count/NUM_TEST_CASES);
	return ret;
}
