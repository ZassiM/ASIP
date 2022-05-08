/*
 * AES (Rijndael) cipher - encrypt
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

// SUBBYTE LOOKUP TABLE
const u32 Te0[256] = {
    0xc66363a5U, 0xf87c7c84U, 0xee777799U, 0xf67b7b8dU,
    0xfff2f20dU, 0xd66b6bbdU, 0xde6f6fb1U, 0x91c5c554U,
    0x60303050U, 0x02010103U, 0xce6767a9U, 0x562b2b7dU,
    0xe7fefe19U, 0xb5d7d762U, 0x4dababe6U, 0xec76769aU,
    0x8fcaca45U, 0x1f82829dU, 0x89c9c940U, 0xfa7d7d87U,
    0xeffafa15U, 0xb25959ebU, 0x8e4747c9U, 0xfbf0f00bU,
    0x41adadecU, 0xb3d4d467U, 0x5fa2a2fdU, 0x45afafeaU,
    0x239c9cbfU, 0x53a4a4f7U, 0xe4727296U, 0x9bc0c05bU,
    0x75b7b7c2U, 0xe1fdfd1cU, 0x3d9393aeU, 0x4c26266aU,
    0x6c36365aU, 0x7e3f3f41U, 0xf5f7f702U, 0x83cccc4fU,
    0x6834345cU, 0x51a5a5f4U, 0xd1e5e534U, 0xf9f1f108U,
    0xe2717193U, 0xabd8d873U, 0x62313153U, 0x2a15153fU,
    0x0804040cU, 0x95c7c752U, 0x46232365U, 0x9dc3c35eU,
    0x30181828U, 0x379696a1U, 0x0a05050fU, 0x2f9a9ab5U,
    0x0e070709U, 0x24121236U, 0x1b80809bU, 0xdfe2e23dU,
    0xcdebeb26U, 0x4e272769U, 0x7fb2b2cdU, 0xea75759fU,
    0x1209091bU, 0x1d83839eU, 0x582c2c74U, 0x341a1a2eU,
    0x361b1b2dU, 0xdc6e6eb2U, 0xb45a5aeeU, 0x5ba0a0fbU,
    0xa45252f6U, 0x763b3b4dU, 0xb7d6d661U, 0x7db3b3ceU,
    0x5229297bU, 0xdde3e33eU, 0x5e2f2f71U, 0x13848497U,
    0xa65353f5U, 0xb9d1d168U, 0x00000000U, 0xc1eded2cU,
    0x40202060U, 0xe3fcfc1fU, 0x79b1b1c8U, 0xb65b5bedU,
    0xd46a6abeU, 0x8dcbcb46U, 0x67bebed9U, 0x7239394bU,
    0x944a4adeU, 0x984c4cd4U, 0xb05858e8U, 0x85cfcf4aU,
    0xbbd0d06bU, 0xc5efef2aU, 0x4faaaae5U, 0xedfbfb16U,
    0x864343c5U, 0x9a4d4dd7U, 0x66333355U, 0x11858594U,
    0x8a4545cfU, 0xe9f9f910U, 0x04020206U, 0xfe7f7f81U,
    0xa05050f0U, 0x783c3c44U, 0x259f9fbaU, 0x4ba8a8e3U,
    0xa25151f3U, 0x5da3a3feU, 0x804040c0U, 0x058f8f8aU,
    0x3f9292adU, 0x219d9dbcU, 0x70383848U, 0xf1f5f504U,
    0x63bcbcdfU, 0x77b6b6c1U, 0xafdada75U, 0x42212163U,
    0x20101030U, 0xe5ffff1aU, 0xfdf3f30eU, 0xbfd2d26dU,
    0x81cdcd4cU, 0x180c0c14U, 0x26131335U, 0xc3ecec2fU,
    0xbe5f5fe1U, 0x359797a2U, 0x884444ccU, 0x2e171739U,
    0x93c4c457U, 0x55a7a7f2U, 0xfc7e7e82U, 0x7a3d3d47U,
    0xc86464acU, 0xba5d5de7U, 0x3219192bU, 0xe6737395U,
    0xc06060a0U, 0x19818198U, 0x9e4f4fd1U, 0xa3dcdc7fU,
    0x44222266U, 0x542a2a7eU, 0x3b9090abU, 0x0b888883U,
    0x8c4646caU, 0xc7eeee29U, 0x6bb8b8d3U, 0x2814143cU,
    0xa7dede79U, 0xbc5e5ee2U, 0x160b0b1dU, 0xaddbdb76U,
    0xdbe0e03bU, 0x64323256U, 0x743a3a4eU, 0x140a0a1eU,
    0x924949dbU, 0x0c06060aU, 0x4824246cU, 0xb85c5ce4U,
    0x9fc2c25dU, 0xbdd3d36eU, 0x43acacefU, 0xc46262a6U,
    0x399191a8U, 0x319595a4U, 0xd3e4e437U, 0xf279798bU,
    0xd5e7e732U, 0x8bc8c843U, 0x6e373759U, 0xda6d6db7U,
    0x018d8d8cU, 0xb1d5d564U, 0x9c4e4ed2U, 0x49a9a9e0U,
    0xd86c6cb4U, 0xac5656faU, 0xf3f4f407U, 0xcfeaea25U,
    0xca6565afU, 0xf47a7a8eU, 0x47aeaee9U, 0x10080818U,
    0x6fbabad5U, 0xf0787888U, 0x4a25256fU, 0x5c2e2e72U,
    0x381c1c24U, 0x57a6a6f1U, 0x73b4b4c7U, 0x97c6c651U,
    0xcbe8e823U, 0xa1dddd7cU, 0xe874749cU, 0x3e1f1f21U,
    0x964b4bddU, 0x61bdbddcU, 0x0d8b8b86U, 0x0f8a8a85U,
    0xe0707090U, 0x7c3e3e42U, 0x71b5b5c4U, 0xcc6666aaU,
    0x904848d8U, 0x06030305U, 0xf7f6f601U, 0x1c0e0e12U,
    0xc26161a3U, 0x6a35355fU, 0xae5757f9U, 0x69b9b9d0U,
    0x17868691U, 0x99c1c158U, 0x3a1d1d27U, 0x279e9eb9U,
    0xd9e1e138U, 0xebf8f813U, 0x2b9898b3U, 0x22111133U,
    0xd26969bbU, 0xa9d9d970U, 0x078e8e89U, 0x339494a7U,
    0x2d9b9bb6U, 0x3c1e1e22U, 0x15878792U, 0xc9e9e920U,
    0x87cece49U, 0xaa5555ffU, 0x50282878U, 0xa5dfdf7aU,
    0x038c8c8fU, 0x59a1a1f8U, 0x09898980U, 0x1a0d0d17U,
    0x65bfbfdaU, 0xd7e6e631U, 0x844242c6U, 0xd06868b8U,
    0x824141c3U, 0x299999b0U, 0x5a2d2d77U, 0x1e0f0f11U,
    0x7bb0b0cbU, 0xa85454fcU, 0x6dbbbbd6U, 0x2c16163aU,
};

const u8 rcons[] = {
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};
//#endif /* AES_SMALL_TABLES */
/**
 * Expand the cipher key into the encryption key schedule.
 *
 * @return	the number of rounds for the given cipher key size.
 */
int rijndaelKeySetupEnc(u32 rk[], const u8 cipherKey[])
{
	int i;
	u32 temp;

	rk[0] = GETU32(cipherKey     );
	rk[1] = GETU32(cipherKey +  4);
	rk[2] = GETU32(cipherKey +  8);
	rk[3] = GETU32(cipherKey + 12);
	rk[4] = GETU32(cipherKey + 16);
	rk[5] = GETU32(cipherKey + 20);
	rk[6] = GETU32(cipherKey + 24);
	rk[7] = GETU32(cipherKey + 28);
	
        for (i = 0; i < 7; i++) {
                temp  = rk[7];
                rk[8] = rk[0] ^ TE421(temp) ^ TE432(temp) ^
                        TE443(temp) ^ TE414(temp) ^ RCON(i);
                rk[9] = rk[1] ^ rk[8];
                rk[10] = rk[2] ^ rk[9];
                rk[11] = rk[3] ^ rk[10];
                if (i == 6)
                        return 14;
                temp  = rk[11];
                rk[12] = rk[4] ^ TE411(temp) ^ TE422(temp) ^
                        TE433(temp) ^ TE444(temp);
                rk[13] = rk[5] ^ rk[12];
                rk[14] = rk[6] ^ rk[13];
                rk[15] = rk[7] ^ rk[14];
                rk += 8;
        }

	return -1;
}



static void rijndaelEncrypt(const u32 rk[], int Nr, const u8 pt[16], u8 ct[16])
{
	unsigned long s0, s1, s2, s3, t0, t1, t2, t3;
        int r;

	/*
	 * map byte array block to cipher state
	 * and add initial round key: ADDROUNDKEY
	 */
	s0 = GETU32(pt     ) ^ rk[0];
	s1 = GETU32(pt +  4) ^ rk[1];
	s2 = GETU32(pt +  8) ^ rk[2];
	s3 = GETU32(pt + 12) ^ rk[3];
        
#define ROUND(i,d,s) \
d##0 = TE0(s##0) ^ TE1(s##1) ^ TE2(s##2) ^ TE3(s##3) ^ rk[4 * i]; \
d##1 = TE0(s##1) ^ TE1(s##2) ^ TE2(s##3) ^ TE3(s##0) ^ rk[4 * i + 1]; \
d##2 = TE0(s##2) ^ TE1(s##3) ^ TE2(s##0) ^ TE3(s##1) ^ rk[4 * i + 2]; \
d##3 = TE0(s##3) ^ TE1(s##0) ^ TE2(s##1) ^ TE3(s##2) ^ rk[4 * i + 3]


    r = Nr >> 1;
    for (;;) {
            ROUND(1,t,s);
            rk += 8;
            if (--r == 0)
                    break;
            ROUND(0,s,t);
    }
	

#undef ROUND

    /*
     * apply last round and
     * map cipher state to byte array block:
     */      
    s0 = TE41(t0) ^ TE42(t1) ^ TE43(t2) ^ TE44(t3) ^ rk[0];
    PUTU32(ct     , s0);
    s1 = TE41(t1) ^ TE42(t2) ^ TE43(t3) ^ TE44(t0) ^ rk[1];
    PUTU32(ct +  4, s1);
    s2 = TE41(t2) ^ TE42(t3) ^ TE43(t0) ^ TE44(t1) ^ rk[2];
    PUTU32(ct +  8, s2);
    s3 = TE41(t3) ^ TE42(t0) ^ TE43(t1) ^ TE44(t2) ^ rk[3];
    PUTU32(ct + 12, s3);	
}

/* Allocate space for the aes encryption keys and compute the schedule */
void * aes_encrypt_init(const u8 *key)
{
	u32 *rk;
	int res;
	rk = (u32*)malloc(AES_PRIV_SIZE);
	if (rk == NULL)
		return NULL;
	res = rijndaelKeySetupEnc(rk, key);
	if (res < 0) {
		free(rk);
		return NULL;
	}
	rk[AES_PRIV_NR_POS] = res;
	return rk;
}


void aes_encrypt(void *ctx, const u8 *plain, u8 *crypt)
{
	u32 *rk = (u32*) ctx; 
	rijndaelEncrypt((u32*)ctx, rk[AES_PRIV_NR_POS], plain, crypt); 
}


void aes_encrypt_deinit(void *ctx)
{
	memset(ctx, 0, AES_PRIV_SIZE);
	free(ctx);
}
