#include "my_malloc.h"

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1
static int DEBUG_COUNT;
#define DLOG(fmt, ...) \
  do { \
    if(DEBUG) \
      /* Use ANSI escape sequences to colorize output */ \
      fprintf(stderr,"\x1b[31mDEBUG:%d:\x1b[0m" \
	                      "\x1b[34m%s:\x1b[0m" \
	                      "\x1b[32m%d:\x1b[0m" \
	      fmt"\n", \
	      DEBUG_COUNT++, \
	      __func__, \
	      __LINE__, \
	      __VA_ARGS__);\
  } \
  while(0)

struct chunck_hdr {
  int size;
  void *content_addr;
  void *next;
};

static struct chunck_hdr* free_list_head = NULL;
static int calls = 0;
void* my_malloc(const int size) {
  DLOG("Call number: %d", calls++);
  DLOG("Header size: %lu", sizeof(struct chunck_hdr));
  DLOG("Size: %d", size);
  DLOG("Heap is at: %p", sbrk(0));
  if (size <= 0) {
    DLOG("%s", "Returning address: NULL");
    return NULL;
  }

  intptr_t inc_size = sizeof(struct chunck_hdr) + size;
  DLOG("Will increase heap size by: %lu", inc_size);
  // TODO(adelq): Fail when this fails.
  void* new_brk = sbrk(inc_size);
  struct chunck_hdr* hdr = new_brk;

  hdr->size = size;
  hdr->content_addr = new_brk + sizeof(struct chunck_hdr);
  hdr->next = NULL;

  DLOG("Returning address: %p", hdr->content_addr);
  return hdr->content_addr;
}

void my_free(void* memory) {
  struct chunck_hdr* hdr = memory;
}
