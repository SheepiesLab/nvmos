#ifndef _USERLAND_H_
#define _USERLAND_H_

#include <stdint.h>

#define PROC_EXEC_ADDR 0x40000000
#define PROC_INIT_MMAP_ADDR 0x60000000
#define PROC_KSTACK_ADDR 0xbffc0000
#define PROC_USTACK_ADDR 0xefffc000

typedef struct
{
	file_meta_t *fileMeta;
	nvmos_ptr_t mmap_addr;
	size_t mmapped_blks;
} proc_fdtable_t;

extern void asm_exitkernel(
	uint32_t userESP,
	uint32_t userEIP);

extern void asm_testUserSpace();

#endif