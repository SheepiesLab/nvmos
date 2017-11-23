#include "kernel/stderr.h"
#include <kernel/io/tty.h>

int stderr_putchar(int c)
{
	terminal_setcolor(TTY_COLOR_LIGHT_RED);
	terminal_putchar(c);

	return 0;
}

void stderr_init(FILE *_stderr)
{
	_stderr->fputchar_impl = stderr_putchar;
	stderr = _stderr;
}
