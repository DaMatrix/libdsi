#include <libdsi/asm.h>

    .arm
    .section .itcm

    .align 4
ASM_FUNC fastClear //TODO: benchmark to see whether this is actually faster than fastFill with 0
    mov r3, #0
    movs r2, r1, lsr#5
    beq _fastClear_words_check
    add r2, r0, r2, lsl #5
    push {r6-r11}
    ldr r12, =_fastClear_7zeroes
    ldmia r12, {r6-r12} //load zeroes into r6-r12
_fastClear_chunks:
    stmia r0!, {r3,r6-r12} //write 8 words at a time (r3 is set to 0 anyway)
    cmp r0, r2
    blt _fastClear_chunks
    pop {r6-r11}
_fastClear_words_check:
    ands r2, r1, #0x1F
    bxeq lr
    add r2, r2, r0
_fastClear_words:
    str r3, [r0], #4
    cmp r0, r2
    blt _fastClear_words
    bx lr

    .section .data
    .align 4
_fastClear_7zeroes:
    .word 0, 0, 0, 0, 0, 0, 0

    .end
