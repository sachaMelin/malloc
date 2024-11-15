#ifndef MALLOC_H
#define MALLOC_H
#include <stddef.h>

struct blk_meta
{
    int free;
    size_t size;
    struct blk_meta *next;
    struct blk_meta *prev;
    int is_page;
};

void *my_malloc(size_t size);
void my_free(void *ptr);

#endif /* ! MALLOC_H */
