#ifndef _USERLAND_H_
#define _USERLAND_H_

#include <stdint.h>

extern void asm_exitkernel(
	uint32_t userESP,
	uint32_t userEIP);

extern void asm_testUserSpace();

#endif