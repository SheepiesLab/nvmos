.section text
.global setIDT
.type setIDT, @function


IDT_SIZE:
.hword 0
IDT_BASE_ADDR:
.word 0

setIDT:
   MOV   4(%esp),   %eax
   MOV   %eax,      IDT_BASE_ADDR
   MOV   8(%esp),   %ax
   MOV   %ax,       IDT_SIZE
   LIDT  (IDT_SIZE)
   RET