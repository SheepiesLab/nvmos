#ifndef _USERLAND_H_
#define _USERLAND_H_

#include <stdint.h>

#define PROC_EXEC_ADDR 0x40000000
#define PROC_KSTACK_ADDR 0xbffc0000
#define PROC_USTACK_ADDR 0xefffc000

typedef struct
{
	file_meta_t *fileMeta;
} proc_fdtable_t;

extern void asm_exitkernel(
	uint32_t userESP,
	uint32_t userEIP);

extern void asm_testUserSpace();

#endif