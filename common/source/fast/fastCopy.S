#include <libdsi/asm.h>

    .arm
    .align 4

#ifdef ARM9
    .section .itcm.fastCopy, "ax"
#else
    .section .text.fastCopy, "ax"
#endif

ASM_FUNC fastCopy
    movs r3, r2, lsr#5
    beq _fastCopy_words_check
    add r3, r0, r3, lsl #5
    push {r5-r11}
_fastCopy_chunks:
    ldmia r0!, {r5-r12}
    stmia r1!, {r5-r12}
    cmp r0, r3
    bne _fastCopy_chunks
    pop {r5-r11}
_fastCopy_words_check:
    ands r3, r2, #0x1F
    bxeq lr
    add r3, r3, r0
_fastCopy_words:
    ldr r12, [r0], #4
    str r12, [r1], #4
    cmp r0, r3
    blt _fastCopy_words
    bx lr

    .end
