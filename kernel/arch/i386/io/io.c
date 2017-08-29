//
// Created by benng on 8/26/17.
//

#include <kernel/io/io.h>

extern unsigned char asm_inb(unsigned short int port);

extern unsigned short int asm_inw(unsigned short int port);

extern unsigned int asm_inl(unsigned short int port);

extern void outb(unsigned char value, unsigned short int port);

extern void outw(unsigned short int value, unsigned short int port);

extern void outl(unsigned int value, unsigned short int port);

unsigned char inb(unsigned short int port) {
    return asm_inb(port);
}

unsigned char inb_p(unsigned short int port) {
    return asm_inb(port);
}

unsigned short int inw(unsigned short int port) {
    return asm_inw(port);
}

unsigned short int inw_p(unsigned short int port) {
    return asm_inw(port);
}

unsigned int inl(unsigned short int port) {
    return asm_inl(port);
}

unsigned int inl_p(unsigned short int port) {
    return asm_inl(port);
}

void outb(unsigned char value, unsigned short int port) {
    return asm_outb(port);
}

void outb_p(unsigned char value, unsigned short int port) {
    return asm_outb(port);
}

void outw(unsigned short int value, unsigned short int port) {
    return asm_outw(port);
}

void outw_p(unsigned short int value, unsigned short int port) {
    return asm_outw(port);
}

void outl(unsigned int value, unsigned short int port) {
    return asm_outl(port);
}

void outl_p(unsigned int value, unsigned short int port) {
    return asm_outl(port);
}

