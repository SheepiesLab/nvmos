.section .text

.extern __start_tss
.global asm_setsyscall
.type asm_setsyscall, @function
asm_setsyscall:
    mov $0x174, %ecx
    mov $0x0,   %edx
    mov $0x6,   %eax
    wrmsr

    mov $0x175, 	%ecx
    mov $0x0,   	%edx
    mov 4(%esp),	%eax
    wrmsr

	lea __start_tss,	%ebx
	mov %eax,		4(%ebx)
	mov %eax,		12(%ebx)
	mov %eax,		20(%ebx)

    mov $0x176, %ecx
    mov $0x0,   %edx
    mov 8(%esp),	%eax
    wrmsr

	lea __start_tss,	%ebx
	mov %eax,		32(%ebx)

    ret

.extern nvmos_syscall
.global asm_syscall
.type asm_syscall, @function
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
