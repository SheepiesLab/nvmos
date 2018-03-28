#include <kernel/syscalls/syscalls.h>
#include <stdio.h>

uint32_t nvmos_syscall(
	uint32_t paramsPtr,
	uint32_t paramsSize,
	uint32_t userESP,
	uint32_t userRetAddr)
{
	printf("SYSCALL!\n");
	printf("EAX (paramsPtr):   0x%p\n", (uint64_t)paramsPtr);
	printf("EBX (paramSize):   0x%p\n", (uint64_t)paramsSize);
	printf("ECX (ESP of user): 0x%p\n", (uint64_t)userESP);
	printf("EDX (return addr): 0x%p\n", (uint64_t)userRetAddr);

	if (paramsPtr == 0xdeadbeef)
	{
		printf("HALT!\n");
		while (1)
			;
	}

	return 0xcdedabcd;
}