#ifndef _USERLAND_H_
#define _USERLAND_H_

#include <stdint.h>

extern void nvmos_exitKernel(uint32_t userESP, uint32_t userEIP);
extern void nvmos_returnKernel();

#endif