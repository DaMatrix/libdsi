#include <libdsi/asm.h>

    .arm
    .align 4

    .section .text.getCPSR, "ax"
ASM_FUNC getCPSR
    //load cpsr into r0
    GET_CPSR(r0)

    //return
    bx lr

    .section .text.getSPSR, "ax"
ASM_FUNC getSPSR
    //load spsr into r0
    GET_SPSR(r0)

    //return
    bx lr

    .section .text.getCurrentMode, "ax"
ASM_FUNC getCurrentMode
    //load cpsr into r0
    GET_CPSR(r0)

    //reduce to range 0-31
    and r0, r0, #0x1F

    //return
    bx lr
