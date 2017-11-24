#include <stdio.h>

#if defined(__is_libk)
#include <kernel/io/serial.h>
#endif

int putchar(int ic) {
#if defined(__is_libk)
//    stdout->fputchar_impl(ic);
    write_serial(ic);
#else
    // TODO: Implement stdio and the write system call.
#endif
    return ic;
}
