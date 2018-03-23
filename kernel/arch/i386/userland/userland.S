.section .text

.global asm_exitkernel
.type asm_exitkernel, @function
asm_exitkernel:
	pop %ebx
    pop %ecx
    pop %edx
    sysexit

.global asm_testUserSpace
.type asm_testUserSpace, @function
asm_testUserSpace:
	mov		$0x23,		%ax
    mov     %ax,        %ds
    mov     %ax,        %es
    mov     %ax,        %gs
    mov     %ax,        %fs
    sysenter
	sysenter
	sysenter
	hlt