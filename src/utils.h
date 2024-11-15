#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>

size_t align(size_t size);
size_t round_up(size_t page_syze, size_t size);
void *beware_overflow(void *ptr, size_t nmemb, size_t size);

#endif /* ! UTILS_H */