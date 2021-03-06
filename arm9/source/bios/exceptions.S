#include <libdsi/asm.h>

    .arm
    .section .exceptionVectors

    //these instructions actually handle interrupts
    ldr pc, _vector_reset
    ldr pc, _vector_undefinedInstruction
    ldr pc, _vector_swi
    ldr pc, _vector_prefetchAbort
    ldr pc, _vector_dataAbort
    ldr pc, _vector_addressExceeds26bit
    ldr pc, _vector_interrupt
    ldr pc, _vector_fastInterrupt

    //fields containing the exception handler addresses
ASM_DATA __exectionVectors
ASM_DATA _vector_reset
    .word 0xFFFF0000

ASM_DATA _vector_undefinedInstruction
    .word 0xFFFF0004

ASM_DATA _vector_swi
    .word 0xFFFF0008

ASM_DATA _vector_prefetchAbort
    .word 0xFFFF000C

ASM_DATA _vector_dataAbort
    .word 0xFFFF0010

ASM_DATA _vector_addressExceeds26bit
    .word 0xFFFF0014

ASM_DATA _vector_interrupt
    .word 0xFFFF0018

ASM_DATA _vector_fastInterrupt
    .word 0xFFFF001C

    .section .text.hijackExceptionVectors, "ax"
ASM_FUNC hijackExceptionVectors
    //get control register value
    mrc p15, 0, r1, c1, c0, 0

    //modify bit 13 (exception vectors)
    //from gbatek:
    // 13 Exception Vectors     (0=00000000h, 1=FFFF0000h)

    //check value of parameter r0 (resetToBIOS)
    cmp r0, #0

    //if false, set vector base to 0x00000000
    biceq r1, r1, #(1 << 13)

    //if true, set vector base to 0xFFFF0000
    orrne r1, r1, #(1 << 13)

    //update control register
    mcr p15, 0, r1, c1, c0, 0

    //return
    bx lr

    .section .text.currentVectorBase, "ax"
ASM_FUNC currentVectorBase
    //get control register value
    mrc p15, 0, r0, c1, c0, 0

    //check state of bit 13
    ands r0, r0, #(1 << 13)

    //if non-zero (which is guarenteed to be 1), set r0 to 0xFFFF0000
    ldr r0, =0xFFFF0000

    //if zero, then we don't need to do anything!

    //return
    bx lr

    .end
