//
// Created by benng on 9/12/17.
//

#ifndef NVMOS_ISR_H
#define NVMOS_ISR_H


typedef unsigned int uword_t;

struct interrupt_frame;

__attribute__((interrupt)) void
irqHandler(struct interrupt_frame *frame);

__attribute__((interrupt)) void
interruptHandler(struct interrupt_frame *frame);

__attribute__((interrupt)) void
exceptionHandler(struct interrupt_frame *frame, uword_t error_code);

#endif //NVMOS_ISR_H
