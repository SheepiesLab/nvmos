.section .text

.global asm_exitkernel
.type asm_exitkernel, @function
asm_exitkernel:
	pop %ebx
	mov $0x174, %ecx
	rdmsr
    pop %ecx
    pop %edx
    sysexit

.global asm_testUserSpace
.type asm_testUserSpace, @function
asm_testUserSpace:
    sysenter
	sysenter
	sysenter
	hlt