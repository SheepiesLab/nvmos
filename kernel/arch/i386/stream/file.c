//
// Created by benng on 7/12/17.
//

#include <kernel/file.h>

int file_putc(int character, struct file *stream) {
    if (stream->fputchar_impl == NULL) {
        return EOF;
    }
    stream->fputchar_impl(character);
    return 0;
}