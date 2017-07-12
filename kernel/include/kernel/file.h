//
// Created by benng on 7/12/17.
//

#ifndef NVMOS_FILE_H
#define NVMOS_FILE_H

#include <stdint.h>
#include <stdio.h>

struct file {
    uint32_t fno;
    char mode[5];

    int (*fgetchar_impl)(void);

    int (*fputchar_impl)(int character);
};

#endif //NVMOS_FILE_H
