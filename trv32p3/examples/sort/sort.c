//  file: sort.c

// Called function:  (locate maximum value in array)

#include "stdio.h"


const int WORD_MAX = 0x7fff;

int find_min_location(int A[], int start, int len)
{
    int U = WORD_MAX;
    int loc = -1;
    for (int i = start; i < len; i++) chess_loop_range(2,) {
	if (A[i] < U) {
	    U = A[i];
	    loc = i;
	}
    }
    return loc;
}

// Called function:  (sort array)

void sort(int A[], int len)
{
    for (int i = 0 ; i < len-1; i++) {
	int loc = find_min_location(A,i,len);
	int temp = A[i];
	A[i] = A[loc];
	A[loc] = temp;
    }
}

// Driver :

#define N 10

int some_array[N] = {-15, -20, 30, 40, 60, -300, 35, 45, 285, 80};

#ifdef NO_HOSTEDIO
#define PRINTF(...)
#else
#define PRINTF(...) { printf(__VA_ARGS__); }
#endif

int main()
{
  PRINTF("unsorted:");
  for (int i = 0; i < N; i++) PRINTF(" %d", some_array[i]);
  PRINTF("\n");
  sort(some_array, N);
  PRINTF("sorted:");
  for (int i = 0; i < N; i++) PRINTF(" %d", some_array[i]);
  PRINTF("\n");
  return 0;
}
