.section .text

.global asm_setsyscall
.type asm_setsyscall @function

asm_setsyscall:
    mov $0x174, %ecx
    mov $0x0,   %edx
    mov $0x8,   %eax
    wrmsr
    mov $0x175, %ecx
    mov $0x0,   %edx
    pop %eax
    wrmsr
    mov $0x176, %ecx
    mov $0x0,   %edx
    pop %eax
    wrmsr
    ret

.extern nvmos_syscall
.global asm_syscall
.type asm_syscall @function

asm_syscall:
    push    %edx
    push    %ecx
    push    %ebp
    mov     %esp,   %ebp
    push    %eax
    call    nvmos_syscall
    mov     %ebp,   %esp
    pop     %ebp
    pop     %ecx
    pop     %edx
    sysexit
    