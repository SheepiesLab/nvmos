//
// Created by benng on 7/12/17.
//

#ifndef NVMOS_FILE_H
#define NVMOS_FILE_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

struct file {
    void *file_impl;

    int (*fputchar_impl)(int character);
};


int file_putc(int character, struct file *stream);


#endif //NVMOS_FILE_H
