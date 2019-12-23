#include <libdsi/asm.h>

    .arm
    .align 4

    .section .text.getCPSR, "ax"
ASM_FUNC getCPSR
    //load cpsr into r0
    GET_CPSR(r0)

    //set T bit based on lsb of lr
    and r1, lr, #1
    orr r0, r0, r1, lsl #5

    //return
    bx lr

    .section .text.getSPSR, "ax"
ASM_FUNC getSPSR
    //load spsr into r0
    GET_SPSR(r0)

    //set T bit based on lsb of lr
    and r1, lr, #1
    orr r0, r0, r1, lsl #5

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

    .section .text.switchToMode, "ax"
ASM_FUNC switchToMode
    //load cpsr into r0
    GET_CPSR(r1)

    //clear old mode from cpsr
    bic r1, r1, #0x1F

    //set new mode
    orr r1, r1, r0

    //restore updated cpsr
    SET_CPSR(r1)

    //return
    bx lr

    .end
