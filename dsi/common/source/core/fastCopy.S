#include <libdsi/asm.h>

    .arm

    .align 4
ASM_FUNC fastCopy itcm
    lsr r3, r2, #5
    cmp r3, #0
    beq _fastCopy_words_check
    add r3, r0, r3, lsr #5
    push {r5-r11}
_fastCopy_chunks:
    ldmia r0!, {r5-r12}
    stmia r1!, {r5-r12}
    cmp r0, r3
    bne _fastCopy_chunks
    pop {r5-r11}
_fastCopy_words_check:
    and r3, r2, #0x1F
    cmp r3, #0
    bxeq lr
    add r3, r3, r0
_fastCopy_words:
    ldr r12, [r0], #4
    str r12, [r1], #4
    cmp r0, r3
    bge _fastCopy_words
    bx lr
