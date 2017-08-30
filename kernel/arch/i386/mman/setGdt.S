.section .data

gdtr:
    .hword 0x0
gdtr_addr:
    .int 0x0

.section .text
.global setGDT
.type setGDT,   @function

setGDT:
    MOV     4(%esp),    %eax
    MOV     %eax,       gdtr_addr
    MOV     8(%esp),    %ax
    MOV     %ax,        gdtr
    LGDT    gdtr
    RET

.global reloadGDT
.type reloadGDT,   @function

reloadGDT:
    MOV     $0x10,      %ax
    MOV     %ax,        %ds
    MOV     %ax,        %es
    MOV     %ax,        %fs
    MOV     %ax,        %gs
    MOV     %ax,        %ss
    JMP     $0x08,      $1f
1:
    RET


