#define _GNU_SOURCE

#include "malloc.h"

#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>

#include "utils.h"

void *head = NULL;

static struct blk_meta *request_space(struct blk_meta *last, size_t size)
{
    size_t page_size = sysconf(_SC_PAGESIZE);
    size_t align_struct = align(sizeof(struct blk_meta));
    size_t total_size = size + align_struct;

    size_t chunk_size = round_up(page_size, total_size);
    struct blk_meta *block = mmap(NULL, chunk_size, PROT_READ | PROT_WRITE,
                                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED)
        return NULL;

    block->size = chunk_size - align_struct;
    block->next = NULL;
    block->prev = last;
    block->free = 1;
    block->is_page = 1;

    if (last != NULL)
        last->next = block;

    return block;
}

static void split_block(struct blk_meta *meta, size_t size)
{
    size_t align_struct = align(sizeof(struct blk_meta));
    struct blk_meta *free_block =
        (struct blk_meta *)((char *)meta + size + align_struct);
    free_block->size = meta->size - size - align_struct;
    free_block->free = 1;
    free_block->next = meta->next;
    free_block->prev = meta;
    free_block->is_page = 0;

    meta->size = size;
    meta->next = free_block;
    meta->free = 0;
}

static struct blk_meta *find_free_block(size_t size)
{
    size_t align_struct = align(sizeof(struct blk_meta));
    struct blk_meta *meta = head;
    while (meta->next != NULL)
    {
        if (meta->free && meta->size >= size + align_struct)
        {
            return meta;
        }
        meta = meta->next;
    }
    if (!meta->next)
    {
        if (meta->free && meta->size >= size + align_struct)
        {
            return meta;
        }
        meta = request_space(meta, size + align_struct);
    }

    return meta;
}

void *my_malloc(size_t size)
{
    if (size == 0)
        return NULL;

    struct blk_meta *block;
    size_t align_size = align(size);
    if (align_size == 0)
        return NULL;
    size_t align_struct = align(sizeof(struct blk_meta));

    if (!head)
    {
        block = request_space(NULL, align_size);
        if (!block)
            return NULL;

        head = block;
    }

    block = find_free_block(align_size);
    split_block(block, align_size);

    if (!block)
        return NULL;

    return (char *)block + align_struct;
}

__attribute__((visibility("default"))) void *malloc(size_t size)
{
    return my_malloc(size);
}

static struct blk_meta *merge_free_blocks(struct blk_meta *meta)
{
    size_t align_struct = align(sizeof(struct blk_meta));
    if (meta->next != NULL)
    {
        if (meta->next->free && !meta->next->is_page)
        {
            meta->size += align_struct + meta->next->size;
            meta->next = meta->next->next;
            if (meta->next != NULL)
            {
                meta->next->prev = meta;
            }
        }
    }
    if (meta->prev != NULL)
    {
        if (meta->prev->free && !meta->is_page)
        {
            meta->prev->size += align_struct + meta->size;
            meta->prev->next = meta->next;
            if (meta->next != NULL)
            {
                meta->next->prev = meta->prev;
            }
            return meta->prev;
        }
    }
    return meta;
}

void my_free(void *ptr)
{
    if (!ptr)
        return;

    size_t align_struct = align(sizeof(struct blk_meta));
    struct blk_meta *block = (struct blk_meta *)((char *)ptr - align_struct);
    block->free = 1;

    block = merge_free_blocks(block);

    if (block->is_page)
    {
        if (block->next == NULL)
        {
            if (block->prev != NULL)
            {
                block->prev->next = NULL;
            }
            if (block == head)
            {
                head = NULL;
            }

            munmap(block, block->size + align_struct);
        }
        else
        {
            if (block->next->is_page && block != head)
            {
                block->next->prev = block->prev;
                block->prev->next = block->next;

                munmap(block, block->size + align_struct);
            }
        }
    }
}
__attribute__((visibility("default"))) void free(void *ptr)
{
    my_free(ptr);
}

__attribute__((visibility("default"))) void *realloc(void *ptr, size_t size)
{
    char *c_ptr = ptr;
    return c_ptr + size;
}

void *my_calloc(size_t nmemb, size_t size)
{
    if (size == 0 || nmemb == 0)
        return NULL;

    void *ptr = malloc(nmemb * size);
    char *c_ptr = ptr;

    if (!beware_overflow(ptr, nmemb, size))
        return NULL;

    for (size_t i = 0; i < nmemb * size; i++)
    {
        c_ptr[i] = 0;
    }

    return ptr;
}

__attribute__((visibility("default"))) void *calloc(size_t nmemb, size_t size)
{
    return my_calloc(nmemb,size);
}
