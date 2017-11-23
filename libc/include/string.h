#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void *, const void *, nvmos_size_t);

void *memcpy(void *__restrict, const void *__restrict, nvmos_size_t);

void *memmove(void *, const void *, nvmos_size_t);

void *memset(void *, int, nvmos_size_t);

nvmos_size_t strlen(const char *);

#ifdef __cplusplus
}
#endif

#endif
