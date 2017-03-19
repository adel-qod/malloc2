#include <unistd.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_malloc.h"

// TODO(adelq): Change this so that it won't need type.
#define STR(x) #x
#define EXPECT_EQ(x_type, x,y_type, y, fmt)	\
  do { \
    x_type var1 = (x);				\
    y_type var2 = (y);							\
    if (var1 != var2) { printf("Got (%s)=`"fmt"` but want (%s)=`"fmt"`.\n", STR(x), var1, STR(x), var2);} \
  } \
  while(0)

int main(void) {
  printf("%s\n", "Testing my_malloc.");

  void* heap_start = sbrk(0);
  assert(heap_start != (void*) -1);

  // Code not needed unless `ulimit` is not unlimited.
  //EXPECT_EQ(void*, my_malloc(-100), void*, NULL, "%p");
  //printf("\n");

  // This should fail.
  EXPECT_EQ(void*, my_malloc(0), void*, NULL, "%p");
  printf("\n");

  const int hdr_size = 24;

  // This should return the heap start + hdr size.
  const int alloc_1_size = 4;
  void* alloc_1 = NULL;
  EXPECT_EQ(void*,
	    alloc_1 = my_malloc(alloc_1_size),
	    void*,
	    heap_start + hdr_size,
	    "%p");
  printf("\n");

  // This should return an addr at the end of the last block
  // (alloc + alloc size) + hdr size.
  const int alloc_2_size = 8;
  void* alloc_2 = NULL;
  EXPECT_EQ(void*,
	    alloc_2 = my_malloc(alloc_2_size),
	    void*,
	    alloc_1 + alloc_1_size + hdr_size,
	    "%p");
  printf("\n");

  // This should return the address of alloc_2
  // because it's the one in the free list
  // that's good enough for us.
  my_free(alloc_2);
  my_free(alloc_1);
  const int alloc_3_size = alloc_2_size;
  void* alloc_3 = NULL;
  EXPECT_EQ(void*,
	    alloc_3 = my_malloc(alloc_3_size),
	    void*,
	    alloc_2,
	    "%p");
  printf("\n");

  // We should now get the address of alloc_1 because it's the only element in
  // the free list that fits.
  const int alloc_4_size = alloc_1_size;
  void* alloc_4 = NULL;
  EXPECT_EQ(void*,
	    alloc_4 = my_malloc(alloc_4_size),
	    void*,
	    alloc_1,
	    "%p");
  printf("\n");

  // Free list is empty; test that we'll allocate shit.
  const int alloc_5_size = 4;
  void* alloc_5 = NULL;
  EXPECT_EQ(void*,
	    alloc_5 = my_malloc(alloc_5_size),
	    void*,
	    alloc_2 + alloc_2_size + hdr_size,
	    "%p");
  printf("\n");

  // Free up two blocks of the free list.
  // We'll ask for a chunck larger than anything we have.
  // Its location will have to be the last address of the heap plus hdr size.
  my_free(alloc_1);
  my_free(alloc_2);
  const int alloc_6_size = 9;
  void* alloc_6 = NULL;
  EXPECT_EQ(void*,
	    alloc_6 = my_malloc(alloc_6_size),
	    void*,
	    alloc_5 + alloc_5_size + hdr_size,
	    "%p");
  printf("\n");
  return 0;
}
