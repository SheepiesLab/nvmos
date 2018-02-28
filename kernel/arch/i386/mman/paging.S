.section .data

pageDirPtr:
    .int 0x0

.section .text
.global nvmos_pagingOn
.type nvmos_pagingOn,   @function

setGDT:
    mov     4(%esp),    %eax
    mov     %eax,       %cr3
    mov     %cr0,       %eax
    or      0x80000001, %eax
    mov     %eax,       %cr0
    ret

.global nvmos_pagingOff
.type nvmos_pagingOff,   @function
    mov     %cr0,       %eax
    and     0x7fffffff, %eax
    mov     %eax,       %cr0
    ret
    


