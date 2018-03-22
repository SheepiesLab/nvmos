#include <stdint.h>

extern void asm_setsyscall(
	uint32_t sysenterESP,
	uint32_t sysenterEIP);

extern void asm_syscall();

void nvmos_syscall();