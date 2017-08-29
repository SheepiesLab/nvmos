.section .text
.global setGDT
.type setGDT, @function


GDT_SIZE:
.hword 0
GDT_BASE_ADDR:
.word 0

setGDT:
   MOV   4(%esp),   %eax
   MOV   %eax,      GDT_BASE_ADDR
   MOV   8(%esp),   %ax
   MOV   %ax,       GDT_SIZE
   LIDT  (GDT_SIZE)
   RET
