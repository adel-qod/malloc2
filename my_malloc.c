#include "my_malloc.h"

#include <unistd.h>

#include <assert.h>
#include <stddef.h>
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
  // Content size.
  size_t size;
  // Address where content begins.
  void *content_addr;
  // Pointer to the next entry in the free list.
  void *next;
};

static struct chunck_hdr* free_list_head = NULL;

static struct chunck_hdr* alloc_from_free_list(const size_t size) {
  DLOG("Searching for a block of size: %lu", size);
  if (free_list_head == NULL) {
    DLOG("%s", "Free list is empty");
    return NULL;
  }

  struct chunck_hdr *cur = free_list_head;
  struct chunck_hdr *prev = free_list_head;
  while (cur != NULL) {
    DLOG("Address of current=`%p with size=%lu", cur, cur->size);
    if (cur->size >= size) {
      DLOG("Will return a block with size: %lu", cur->size);
      if (cur == free_list_head) {
	free_list_head = NULL;
      } else {
	prev->next = cur->next;
      }
      cur->next = NULL;
      return cur;
    }
    prev = cur;
    cur = cur->next;
  }

  DLOG("Failed to find a block of size: %lu in the free list", size);
  return NULL;
}

void* my_malloc(const size_t size) {
  static int calls = 1;
  DLOG("Call number: %d", calls++);
  DLOG("Header size: %lu", sizeof(struct chunck_hdr));
  DLOG("Size: %lu", size);
  DLOG("Heap is at: %p", sbrk(0));
  if (size == 0) {
    DLOG("%s", "Size=0; returning address: NULL");
    return NULL;
  }

  const size_t total_size = sizeof(struct chunck_hdr) + size;

  void* block_addr = NULL;
  block_addr = alloc_from_free_list(size);
  if (block_addr == NULL) {
    // Ask the kernel to give us more heap space.
    block_addr = sbrk(total_size);
    if (block_addr == (void*) -1) {
      DLOG("%s\n", "Failed to increase heap; returning address: NULL");
      return NULL;
    }
    DLOG("Increased heap size by: %lu", total_size);
  }

  struct chunck_hdr* hdr = block_addr;
  hdr->size = size;
  hdr->content_addr = block_addr + sizeof(struct chunck_hdr);
  hdr->next = NULL;

  DLOG("Returning address: %p", hdr->content_addr);
  return hdr->content_addr;
}

void my_free(void* memory) {
  struct chunck_hdr* hdr = memory - sizeof(struct chunck_hdr);
  hdr->next = free_list_head;
  free_list_head = hdr;
  DLOG("Freed a block of size: %lu", hdr->size);
}
