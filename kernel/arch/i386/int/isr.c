//
// Created by benng on 9/12/17.
//
#include <kernel/int/isr.h>
#include <kernel/int/pic.h>
#include <kernel/io/serial.h>
#include <stdio.h>


void irqHandler(struct interrupt_frame *frame)
{
	printf("IRQ4");
	PIC_sendEOI(4);
}

void interruptHandler(struct interrupt_frame *frame)
{
	printf("IH\n");
}

void exceptionHandler(struct interrupt_frame *frame, uword_t error_code)
{
	printf("EH\n");
}
