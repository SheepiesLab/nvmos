#include "kernel/stdout.h"
#include <kernel/tty.h>

int stdout_putchar(int c) {
    terminal_setcolor(TTY_COLOR_LIGHT_GREY);
    terminal_putchar(c);
}

void stdout_init(FILE *_stdout) {
    _stdout->fputchar_impl = stdout_putchar;
    stdout = _stdout;
}
