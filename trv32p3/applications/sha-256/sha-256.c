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



#include <stdio.h> 
#include <string.h> 


// Preprocessor defines for the test cases, which are stored in the .dat files. "K" ist constant
#define NUM_TESTCASES 7
#define START_ADDRESS 1000
#define NUM_INPUT_BYTES 172


// Preprocessor function defined to ease the readability of the SHA-256 code
// Functions are called according to common naming conventions of SHA-256
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))


// Declare variables at predefined memory locations to allow the program to access the data that 
// is written at those locations by the simulation tool from the .dat files

static unsigned char chess_storage(DMb:START_ADDRESS) sha_input[NUM_INPUT_BYTES];
static int chess_storage(DMb:(START_ADDRESS+NUM_INPUT_BYTES)) sha_input_lengths[NUM_TESTCASES];
static int chess_storage(DMb:(START_ADDRESS+NUM_INPUT_BYTES+4*NUM_TESTCASES)) k[64];
static unsigned char chess_storage(DMb:(START_ADDRESS+NUM_INPUT_BYTES+4*NUM_TESTCASES+ 64*4)) expected_hashes[NUM_TESTCASES][32]; 


/* ---------------------------------------------------------------------------------------- */
//                                  SHA256_TRANSFORM
//              Function that defines the main hashing function of SHA-256 
//  Inputs: 1. unsigned character array: data  (carries a 64 byte block of data to be hashed)
//  Input + Output 1.:-9. unsigned integer:h0-h7 
//              (As input: Current or initial hashing values. As output: New hashing values) 
//  HASH h (h0-h7) 32 byte in total...
/* ---------------------------------------------------------------------------------------- */
void sha256_transform(unsigned char data[],  unsigned int* h0, unsigned int* h1, unsigned int* h2, unsigned int* h3, unsigned int* h4, unsigned int* h5, unsigned int* h6, unsigned int* h7)
{  
    // Declare local variables:
    // a-h: Local intermediate hash values
    // t1&t2: temp values
    // i: Loop counter and index
    
    unsigned int a,b,c,d,e,f,g,h,i,j,t1,t2;
    unsigned int m[64];// = (unsigned int*)&data[0];
    for (i=0,j=0; i < 16; ++i, j += 4)
     m[i] = (data[j] << 24) | (data[j+1] << 16) | (data[j+2] << 8) | (data[j+3]);
    a = *h0;
    b = *h1;
    c = *h2;
    d = *h3;
    e = *h4;
    f = *h5;
    g = *h6;
    h = *h7;

    for ( i = 16; i < 64; ++i) m[i] = SIG1(m[i-2]) + m[i-7] + SIG0(m[i-15]) + m[i-16];

    for (i = 0; i < 64; ++i) 
    {
        t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
        t2 = EP0(a) + MAJ(a,b,c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
      
    *h0 += a;
    *h1 += b;
    *h2 += c;
    *h3 += d;
    *h4 += e;
    *h5 += f;
    *h6 += g;
    *h7 += h;
}  

void sha256(unsigned char data[], unsigned int len, unsigned char hash[])
{  
   unsigned int t;
   unsigned char buf[64*4];

 // Initialize state of SHA-256
 
   unsigned int a = 0x6a09e667;
   unsigned int b = 0xbb67ae85;
   unsigned int c = 0x3c6ef372;
   unsigned int d = 0xa54ff53a;
   unsigned int e = 0x510e527f;
   unsigned int f = 0x9b05688c;
   unsigned int g = 0x1f83d9ab;
   unsigned int h = 0x5be0cd19;
  
 // Process message
   unsigned int datalen = len; 
   unsigned char* pdata = data;
   
 int block_offset = 0;
 
 for(block_offset = 0; len - block_offset >=64; block_offset += 64)
 {
     for(int i = 0; i < 64; i++)
     {
            buf[i] = *pdata++;
     }
     sha256_transform(buf ,&a,&b,&c,&d,&e,&f,&g,&h);
     printf("Block Offset: %d\n", block_offset);	
 }
 pdata += block_offset;
 datalen -= block_offset;
 unsigned int i = 0;
 while(i < datalen)
 {
     buf[i++] = *pdata++;
 }
  // Pad whatever data is left in the buffer.
  if (datalen < 56) {
    buf[i++] = 0x80;
    while (i < 56)
      buf[i++] = 0x00;
  }
  else {
    buf[i++] = 0x80;
    while (i < 64)
      buf[i++]= 0x00;
    sha256_transform(buf ,&a,&b,&c,&d,&e,&f,&g,&h);
    memset(buf,0,56);
  }
   
   // Append to the padding the total message's length in bits and transform. 
   unsigned long long bitlen = len << 3; 
   
   buf[63] = bitlen;
   buf[62] = bitlen >> 8;
   buf[61] = bitlen >> 16;
   buf[60] = bitlen >> 24;
   buf[59] = bitlen >> 32;
   buf[58] = bitlen >> 40;
   buf[57] = bitlen >> 48;
   buf[56] = bitlen >> 56;
   
   sha256_transform(buf,&a,&b,&c,&d,&e,&f,&g,&h);
   
   // Since this implementation uses little endian byte ordering and SHA uses big endian,
   // reverse all the bytes when copying the final state to the output hash. 
   for (i=0; i < 4; ++i) { 
      hash[i]    = (a >> (24-i*8)) & 0x000000ff; 
      hash[i+4]  = (b >> (24-i*8)) & 0x000000ff; 
      hash[i+8]  = (c >> (24-i*8)) & 0x000000ff;
      hash[i+12] = (d >> (24-i*8)) & 0x000000ff;
      hash[i+16] = (e >> (24-i*8)) & 0x000000ff;
      hash[i+20] = (f >> (24-i*8)) & 0x000000ff;
      hash[i+24] = (g >> (24-i*8)) & 0x000000ff;
      hash[i+28] = (h >> (24-i*8)) & 0x000000ff;
   }  
}  


void print_hash(unsigned char hash[])
{
   int idx;
   printf("Resulting Hash: 0x");
   for (idx=0; idx < 32; idx++)
      printf("%02x",hash[idx]);
   printf("\n");
}

int cmp_hash(unsigned char* a, unsigned char*b, int length)
{
    int difference = 0;
    for(int i = 0; i < length; i++)
    {
        if((a[i]&0xff) != (b[i]&0xff)) difference++;
    }
    return difference;
}
int main()
{
    unsigned char hash[NUM_TESTCASES][32];
    int len;
    char buf[64];
    
    int input_lengths_rounded_up[NUM_TESTCASES];
    for (int i = 0; i < NUM_TESTCASES; i++)
    {   input_lengths_rounded_up[i] = ((sha_input_lengths[i]+3)/4)*4;
        printf("%d   %d\n",input_lengths_rounded_up[i], sha_input_lengths[i]);
    }
    int num_correct_test_cases = 0;

    int current_sha_input_start = 0;
    int starting_cycle_count = chess_cycle_count();
    
    len = sha_input_lengths[0];
    sha256(&sha_input[0],len,hash[0]); // "" empty string input

    current_sha_input_start += input_lengths_rounded_up[0];
    len = sha_input_lengths[1]; 
    sha256(&sha_input[current_sha_input_start],len,hash[1]); // "a"

    current_sha_input_start += input_lengths_rounded_up[1];
    len = sha_input_lengths[2]; 
    sha256(&sha_input[current_sha_input_start],len,hash[2]); // "abc"

    current_sha_input_start += input_lengths_rounded_up[2];
    len = sha_input_lengths[3]; 
    sha256(&sha_input[current_sha_input_start],len,hash[3]); // "message digest"

    current_sha_input_start += input_lengths_rounded_up[3];
    len = sha_input_lengths[4]; 
    sha256(&sha_input[current_sha_input_start],len,hash[4]); //"abcdefghijklmnopqrstuvwxyz"

    current_sha_input_start += input_lengths_rounded_up[4];
    len = sha_input_lengths[5]; 
    sha256(&sha_input[current_sha_input_start],len,hash[5]); //"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"

    current_sha_input_start += input_lengths_rounded_up[5];
    len = sha_input_lengths[6]; 
    sha256(&sha_input[current_sha_input_start],len,hash[6]); //"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopqabcdabcd"
    int finish_cycle_count = chess_cycle_count();
    
    
    int current_index = 0;
    for(int i = 0; i < NUM_TESTCASES; i++)
    {
        printf("Testcase %d (Input Length: %d bytes):\n", i, sha_input_lengths[i]);
        
        printf("SHA Input: 0x");
        for (int idx=0; idx < input_lengths_rounded_up[i]; idx++)
            printf("%02x",sha_input[current_index + idx]);
        printf("\n");
        
        print_hash(hash[i]);
        if(cmp_hash(hash[i], expected_hashes[i], 32) == 0)
        {
            printf("Testcase %d successful!\n", i);
            num_correct_test_cases++;
        }
        else
        {
            printf("Testcase %d unsuccessful! Expected hash is:\n",i);
            printf("Expected Hash: 0x");
            for(int j = 0; j < 32; j++)
            {
                printf("%x", expected_hashes[i][j]&0xff);
            }
            printf("\n");
        }
        printf("\n\n");
        current_index = current_index + input_lengths_rounded_up[i];
    }


    printf("Total Cycle Count: %d\n", finish_cycle_count - starting_cycle_count);
    printf("Average Cycle Count per Hash: %d\n\n", (finish_cycle_count - starting_cycle_count)/NUM_TESTCASES);
    printf("%d of %d test cases correct.\n", num_correct_test_cases, NUM_TESTCASES);
    return 0;
}

