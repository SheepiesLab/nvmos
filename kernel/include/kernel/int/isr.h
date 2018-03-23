//
// Created by benng on 9/12/17.
//

#ifndef NVMOS_ISR_H
#define NVMOS_ISR_H

extern void asm_inthand();

extern void asm_irqhand();

extern void asm_execpthand();

void irqHandler();

void interruptHandler();

void exceptionHandler();

#endif //NVMOS_ISR_H
