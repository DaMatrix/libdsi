#include <libdsi/asm.h>

ASM_FUNC fastClear
    add r2, r0, r1
    cmp r2, r0
    bxeq lr //immediately return if 0 bytes
    //TODO: this isn't actually fast
    mov r3, #0
_fastClear_body:
    str r3, [r0], #4
    cmp r0, r2
    blt _fastClear_body
    bx lr
