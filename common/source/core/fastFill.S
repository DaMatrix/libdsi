#include <libdsi/asm.h>

    .arm
#ifdef ARM9
    .section .itcm
#endif

    .align 4
ASM_FUNC fastFillHalfWords
    orr r0, r0, r0, lsl#16

ASM_FUNC fastFill
    movs r3, r2, lsr#5
    beq _fastFill_words_check
    add r3, r1, r3, lsl #5
    push {r6-r11}
    mov r6, r0
    mov r7, r0
    mov r8, r0
    mov r9, r0
    mov r10, r0
    mov r11, r0
    mov r12, r0
_fastFill_chunks:
    stmia r1!, {r0,r6-r12}
    cmp r1, r3
    blt _fastFill_chunks
    pop {r6-r11}
_fastFill_words_check:
    ands r3, r2, #0x1F
    bxeq lr
    add r3, r3, r1
_fastFill_words:
    str r0, [r1], #4
    cmp r1, r3
    blt _fastFill_words
    bx lr

    .end
