.section .text
.global setGDT
.type setGDT, @function


GDT_SIZE:
.hword 0
GDT_BASE_ADDR:
.word 0

setGDT:
   MOV   %eax, 4(%esp)
   MOV   GDT_BASE_ADDR, %eax
   MOV   %ax, 8(%esp)
   MOV   GDT_SIZE, %ax
   LGDT  (GDT_SIZE)
   RET
