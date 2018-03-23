//
// Created by benng on 9/12/17.
//
#include <kernel/int/isr.h>
#include <kernel/int/pic.h>
#include <kernel/io/serial.h>
#include <stdio.h>

void irqHandler()
{
	printf("IRQ4");
	PIC_sendEOI(4);
}

void interruptHandler()
{
	printf("IH\n");
}

void exceptionHandler()
{
	printf("EH\n");
}
