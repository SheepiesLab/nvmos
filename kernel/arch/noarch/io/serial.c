//
// Created by benng on 8/29/17.
//

#include <kernel/io/io.h>
#include <kernel/io/serial.h>

#define PORT 0x3f8   /* COM1 */

void init_serial() {
    outb(0x00, PORT + 3);    // Disable DLAB (set baud rate divisor)
    outb(0x00, PORT + 1);    // Disable all interrupts
    outb(0x80, PORT + 3);    // Enable DLAB (set baud rate divisor)
    outb(0x01, PORT + 0);    // Set divisor to 1 (lo byte) 115200 baud
    outb(0x00, PORT + 1);    //                  (hi byte)
    outb(0x03, PORT + 3);    // 8 bits, no parity, one stop bit
    outb(0x07, PORT + 2);    // Enable FIFO, clear them, with 1-byte threshold
    outb(0x0B, PORT + 4);    // IRQs enabled, RTS/DSR set
    outb(0x0F, PORT + 1);    // Enable avail interrupts
}

int serial_received() {
    return inb(PORT + 5) & 1;
}

char read_serial() {
    while (serial_received() == 0);

    return inb(PORT);
}

int is_transmit_empty() {
    return inb(PORT + 5) & 0x40;
}

void write_serial(char a) {
    while (is_transmit_empty() == 0);

    outb(a, PORT);
}