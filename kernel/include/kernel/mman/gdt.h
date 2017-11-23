#ifndef NVMOS_GDT_H
#define NVMOS_GDT_H

#include <stdint.h>
#include <stddef.h>

#define GDT_GD_TYPE_PR 0x80
#define GDT_GD_TYPE_PRIVI_0 0x00   //0 is highest
#define GDT_GD_TYPE_PRIVI_1 0x20
#define GDT_GD_TYPE_PRIVI_2 0x40
#define GDT_GD_TYPE_PRIVI_3 0x60
#define GDT_GD_TYPE_BIT4 0x10 //Bit 4 of access byte must be set
#define GDT_GD_TYPE_EX 0x8
#define GDT_GD_TYPE_DC 0x4
#define GDT_GD_TYPE_RW 0x2
#define GDT_GD_TYPE_AC 0x1

#define GDT_GD_FLAG_GR 0x80
#define GDT_GD_FLAG_SZ 0x40


typedef struct gd {
    uint32_t base;
    uint32_t limit; //Limit only has 20bits. Check for it.
    uint8_t type;
} GlobalDescriptor;

int gd_fillEntry(
        GlobalDescriptor *gd,
        uint32_t base,
        uint32_t limit,
        uint8_t type
);

int gdt_commit(uint8_t *buffer, GlobalDescriptor *gdt, nvmos_size_t gdtLen);

int gd_encode(uint8_t *buffer, GlobalDescriptor gd);

#endif