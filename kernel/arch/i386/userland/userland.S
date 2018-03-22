.section .text

.global asm_exitkernel
.type asm_exitkernel @function

asm_exitkernel:
    pop %ecx
    pop %edx
    sysexit

    