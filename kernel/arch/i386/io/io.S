.section text

.global asm_inb
.type asm_inb, @function

asm_inb:
    MOV     4(%esp),    %dx
    IN      %dx,        %al
    RET

.global asm_inw
.type asm_inw, @function

asm_inw:
    MOV     4(%esp),    %dx
    IN      %dx,        %ax
    RET

.global asm_inl
.type asm_inl, @function

asm_inl:
    MOV     4(%esp),    %dx
    IN      %dx,        %eax
    RET

.global asm_outb
.type asm_outb, @function

asm_outb:
    MOV     4(%esp),    %al
    MOV     5(%esp),    %dx
    out     %al,        %dx
    RET

.global asm_outw
.type asm_outw, @function

asm_outw:
    MOV     4(%esp),    %dx
    MOV     6(%esp),    %ax
    out     %ax,        %dx
    RET

.global asm_outl
.type asm_outl, @function

asm_outl:
    MOV     4(%esp),    %dx
    MOV     8(%esp),    %eax
    out     %eax,       %dx
    RET
