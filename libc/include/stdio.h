#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <kernel/file.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct file FILE;

FILE *stdin;
FILE *stdout;
FILE *stderr;

int printf(const char *__restrict, ...);

int putchar(int);

int puts(const char *);

#ifdef __cplusplus
}
#endif

#endif
