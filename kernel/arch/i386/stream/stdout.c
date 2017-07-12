#include "kernel/stdout.h"
#include <kernel/tty.h>

int stdout_putchar(int c) {
    terminal_setcolor(TTY_COLOR_LIGHT_GREY, TTY_COLOR_BLACK);
    terminal_write(&c, sizeof(c));
}

void stdout_init(FILE *_stdout) {
    _stdout->fno = 1;
    _stdout->mode = "a";
    _stdout->fputchar_impl = stdout_putchar;
    stdout = _stdout;
}
