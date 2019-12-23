#include <libdsi/asm.h>

//functions for switching between different stack modes

    .arm
    .align 4

    .section .text.switchToInternalStack, "ax"
ASM_FUNC switchToInternalStack
    //save original cpsr
    GET_CPSR(r2)

    ldr r1, =__internal_dummy_stack
    add r1, r1, #(1024 * 3)

    //arm interrupt mode
    mov r0, #0x12
    SET_CPSR(r0)
    mov sp, r1

    //arm supervisor mode
    mov r0, #0x13
    SET_CPSR(r0)
    sub r1, r1, #0x400
    mov sp, r1

    //arm system mode
    mov r0, #0x1F
    SET_CPSR(r0)
    sub r1, r1, #0x400
    mov sp, r1

    //restore original cpsr
    SET_CPSR(r2)

    //return
    bx lr

    .section .text.switchToDTCMStack, "ax"
ASM_FUNC switchToDTCMStack
    //save original cpsr
    GET_CPSR(r2)

    //arm interrupt mode
    mov r0, #0x12
    SET_CPSR(r0)
    ldr sp, =__sp_irq

    //arm supervisor mode
    mov r0, #0x13
    SET_CPSR(r0)
    ldr sp, =__sp_svc

    //arm system mode
    mov r0, #0x1F
    SET_CPSR(r0)
    ldr sp, =__sp_usr

    //restore original cpsr
    SET_CPSR(r2)

    //return
    bx lr

    .section .text.switchToDebugStack, "ax"
ASM_FUNC switchToDebugStack
    //save original cpsr
    GET_CPSR(r2)

    //this is actually different than what the debug stack should be, but this is the code that libnds uses...
    //TODO: i don't think this should actually work correctly on DSi, figure out some other place i can put this...
    //https://problemkaputt.de/gbatek.htm#biosramusage

    mov r1, #0x03000000

    //arm interrupt mode
    mov r0, #0x12
    SET_CPSR(r0)
    sub r1, r1, #0x1000
    mov sp, r1

    //arm supervisor mode
    mov r0, #0x13
    SET_CPSR(r0)
    sub r1, r1, #0x100
    mov sp, r1

    //arm system mode
    mov r0, #0x1F
    SET_CPSR(r0)
    sub r1, r1, #0x100
    mov sp, r1

    //restore original cpsr
    SET_CPSR(r2)

    //return
    bx lr
