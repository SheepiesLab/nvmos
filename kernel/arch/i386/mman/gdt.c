#include <kernel/mman/gdt.h>

int gd_fillEntry(GlobalDescriptor *gd, uint32_t base, uint32_t limit, uint8_t type) {
    gd->base = base;
    gd->limit = limit;
    gd->type = type;
}

int gd_encode(uint8_t *buffer, GlobalDescriptor GD) {
    // Check the limit to make sure that it can be encoded
    if ((GD.limit > 65536) && ((GD.limit & 0xFFF) != 0xFFF)) {
        return -1;
    }
    if (GD.limit > 65536) {
        // Adjust granularity if required
        GD.limit = GD.limit >> 12;
        buffer[6] = GDT_GD_FLAG_GR | GDT_GD_FLAG_SZ;
    } else {
        buffer[6] = GDT_GD_FLAG_SZ;
    }

    // Encode the limit
    buffer[0] = GD.limit & 0xFF;
    buffer[1] = (GD.limit >> 8) & 0xFF;
    buffer[6] |= (GD.limit >> 16) & 0xF;

    // Encode the base
    buffer[2] = GD.base & 0xFF;
    buffer[3] = (GD.base >> 8) & 0xFF;
    buffer[4] = (GD.base >> 16) & 0xFF;
    buffer[7] = (GD.base >> 24) & 0xFF;

    // And... Type
    buffer[5] = GD.type;

    return 0;
}

extern void setGDT(uint32_t base, uint16_t size);

int gdt_commit(uint8_t *buffer, GlobalDescriptor *gdt, size_t gdtLen) {
    if (gdtLen > 0xFFFF)
        return -1;

    for (int i = 0; i < gdtLen; ++i) {
        if (gd_encode(buffer + (16 * i), gdt[i]) == -1)
            return -1;
    }

    uint32_t gdtBase = (uint32_t) buffer;
    uint16_t gdtSize = gdtLen;
    setGDT(gdtBase, gdtSize);

    return 0;
}
