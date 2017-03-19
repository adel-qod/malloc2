#include <unistd.h>

#include <assert.h>
#include <stdio.h>

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
  printf("%s\n", "Testing malloc.");

  void* heap_start = sbrk(0);
  assert(heap_start != (void*) -1);

  EXPECT_EQ(void*, my_malloc(-100), void*, NULL, "%p");
  printf("\n");

  EXPECT_EQ(void*, my_malloc(0), void*, NULL, "%p");
  printf("\n");

  const int hdr_size = 24;

  const int alloc_1_size = 4;
  void* alloc_1 = NULL;
  EXPECT_EQ(void*, alloc_1 = my_malloc(alloc_1_size), void*, heap_start + hdr_size, "%p");
  printf("\n");

  const int alloc_2_size = 8;
  void* alloc_2 = NULL;
  EXPECT_EQ(void*, alloc_2 = my_malloc(alloc_2_size), void*, alloc_1 + alloc_1_size + hdr_size, "%p");
  printf("\n");

  return 0;
}
