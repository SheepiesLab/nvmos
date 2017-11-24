//
// Created by benng on 9/12/17.
// See http://wiki.osdev.org/Interrupt_Descriptor_Table
//

#include <kernel/int/idt.h>

struct intDescr {
	uint16_t	offset_1;   // offset bits 0..15
	uint16_t	selector;   // a code segment selector in GDT or LDT
	uint8_t		zero;       // unused, set to 0
	uint8_t		type_attr;  // type and attributes
	uint16_t	offset_2;   // offset bits 16..31
};

int interruptDescriptor_Encode(
	InterruptDescriptor *	interruptDescriptor,
	IDTEntry *				IDTEntry)
{
	IDTEntry->zero = 0;
	IDTEntry->offset_1 = interruptDescriptor->isrAddr & 0x0000FFFF;
	IDTEntry->offset_2 = interruptDescriptor->isrAddr >> 16;
	IDTEntry->selector = 0x08;
	IDTEntry->type_attr = interruptDescriptor->type + 0x80;

	return 0;
}
