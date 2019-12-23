#include <libdsi/asm.h>

    .arm
    .align 4

    .section .text.checkDSiMode, "ax"
ASM_FUNC checkDSiMode
    //register base
    mov r1, #0x4000000

    //read SCFG_A9ROM
    ldr r0, [r1, r1, lsr#12]

    //reduce to 2 lsbs
    and r0, r0, #0x3

    //any value other than 1 is invalid
    cmp r0, #1
    movne r0, #0

    //return
    bx lr

    .end
