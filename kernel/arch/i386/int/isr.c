//
// Created by benng on 9/12/17.
//
#include <kernel/int/isr.h>
#include <kernel/int/pic.h>
#include <kernel/io/serial.h>
#include <stdio.h>


__attribute__((interrupt)) void
irqHandler(struct interrupt_frame *frame) {
    printf("IRQ4");
    PIC_sendEOI(4);
}

__attribute__((interrupt)) void
interruptHandler(struct interrupt_frame *frame) {
    printf("IH\n");
}

__attribute__((interrupt)) void
exceptionHandler(struct interrupt_frame *frame, uword_t error_code) {
    printf("EH\n");
}