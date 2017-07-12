#include "kernel/stderr.h"
#include <kernel/tty.h>

int stderr_putchar(int c) {
    terminal_setcolor(TTY_COLOR_LIGHT_RED, TTY_COLOR_BLACK);
    terminal_write(&c, sizeof(c));
}

void stderr_init(FILE *_stderr) {
    _stderr->fno = 1;
    _stderr->mode = "a";
    _stderr->fputchar_impl = stderr_putchar;
    stderr = _stderr;
}