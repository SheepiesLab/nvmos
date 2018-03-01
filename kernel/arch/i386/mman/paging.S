.section .text

.global nvmos_pagingOn
.type nvmos_pagingOn,   @function
nvmos_pagingOn:
    mov     4(%esp),    %eax
    mov     %eax,       %cr3
    mov     %cr0,       %eax
    or      %eax,       0x80000001
    mov     %eax,       %cr0
    ret

.global nvmos_pagingOff
.type nvmos_pagingOff,   @function
nvmos_pagingOff:
    mov     %cr0,       %eax
    and     0x7fffffff, %eax
    mov     %eax,       %cr0
    ret
    


