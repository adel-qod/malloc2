#ifndef SRC_MALLOC_MY_MALLOC
#define SRC_MALLOC_MY_MALLOC

#include <stddef.h>

// In linux; as with malloc, calling this will never fail by default.
// The kernel will return a heap address and caller will segfault upon
// accessing the page.
void* my_malloc(const size_t size);
// Does not block.
void my_free(void* memory);

#endif
