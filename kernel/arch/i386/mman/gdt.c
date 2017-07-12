#include <kernel/gdt.h>

int encodeGD(uint8_t *buffer, GlobalDescriptor GD) {
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

int commitGDT(uint8_t *buffer, GlobalDescriptor *GDT, size_t GDTSize) {
    if (GDTSize > 0xFFFF)
        return -1;

    for (int i = 0; i < GDTSize; ++i) {
        if (encodeGD(buffer + (16 * i), GDT[i]) == -1)
            return -1;
    }

    uint32_t gdtBase = (uint32_t) buffer;
    uint16_t gdtSize = GDTSize;
    setGDT(gdtBase, gdtSize);

    return 0;
}
