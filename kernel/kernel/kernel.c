#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void* multibootInfo) {
	terminal_initialize();
	printf("Hello, kernel World!\nI'm new!\n");
	printf("%p\n", multibootInfo);
}
