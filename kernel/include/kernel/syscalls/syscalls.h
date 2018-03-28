#ifndef _NVMOS_SYSCALL_H
#define _NVMOS_SYSCALL_H
#include <stdint.h>

extern void asm_setsyscall(
	uint32_t sysenterESP,
	uint32_t sysenterEIP);

extern void asm_syscall();

uint32_t nvmos_syscall(
	uint32_t paramsPtr,
	uint32_t paramsSize,
	uint32_t userESP,
	uint32_t userRetAddr);

#endif