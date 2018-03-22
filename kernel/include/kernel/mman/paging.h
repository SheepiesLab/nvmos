#ifndef _NVMOS_PAGING_H_
#define _NVMOS_PAGING_H_


#include <stdint.h>

extern void nvmos_pagingOn(uint32_t pageDir);

extern void nvmos_pagingOff();

#endif