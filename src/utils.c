#include "utils.h"

size_t align(size_t size)
{
    size_t max = sizeof(long double);
    size_t r = size % max;

    size_t padding = 0;
    if (r != 0)
        padding = max - r;

    size_t res;
    if (__builtin_add_overflow(size, padding, &res))
        return 0;

    return res;
}

size_t round_up(size_t page_syze, size_t size)
{
    size_t i = 1;
    while ((page_syze * i) < size)
    {
        i++;
    }
    return page_syze * i;
}

void *beware_overflow(void *ptr, size_t nmemb, size_t size)
{
    size_t res;
    if (__builtin_mul_overflow(size, nmemb, &res))
    {
        return NULL;
    }

    char *tmp = ptr;
    tmp += res;

    return tmp;
}
