.section .text

.global asm_exitkernel
.type asm_exitkernel @function

asm_exitkernel:
    MOV $0x174, %ecx
    MOV $0x0,   %edx
    MOV $0x8,   %eax
    WRMSR
    