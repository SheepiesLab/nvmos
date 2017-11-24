#ifndef NVMOS_IDT_H
#define NVMOS_IDT_H

#include <stdint.h>
#include <stdbool.h>

extern void setIDT(uint32_t base, uint16_t size);

#define ID_TYPE_386_TASK    0x5
#define ID_TYPE_386_INT     0xE
#define ID_TYPE_386_TRAP    0xF


typedef struct interruptDescriptor {
    uint32_t isrAddr;
    bool present;
    uint8_t privilegeLevel; // 0 highest, 0x11 lowest
    uint8_t type;
} InterruptDescriptor;

typedef struct intDescr IDTEntry;// 8 byte

int interruptDescriptor_Encode(
        InterruptDescriptor *interruptDescriptor,
        IDTEntry *IDTEntry);

#endif //NVMOS_IDT_H