//
// Created by benng on 9/12/17.
//

#ifndef NVMOS_ISR_H
#define NVMOS_ISR_H


typedef unsigned int uword_t;

struct interrupt_frame;

void irqHandler(struct interrupt_frame *frame);

void interruptHandler(struct interrupt_frame *frame);

void exceptionHandler(struct interrupt_frame *frame, uword_t error_code);

#endif //NVMOS_ISR_H
