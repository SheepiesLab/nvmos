.section .text

.extern irqHandler
.extern interruptHandler
.extern exceptionHandler

.global asm_inthand
.align 4
.type asm_inthand @function

asm_inthand:
	pushal
	cld
	call interruptHandler
	popal
	iret

.global asm_irqhand
.align 4
.type asm_irqhand @function

asm_irqhand:
	pushal
	cld
	call irqHandler
	popal
	iret

.global asm_execpthand
.align 4
.type asm_execpthand @function

asm_execpthand:
	pushal
	cld
	call exceptionHandler
	popal
	add	$0x4,	%esp
	iret