#include <libdsi/asm.h>

ASM_FUNC crashSystemNoMsg
    mov r0, #0

ASM_FUNC crashSystem
    //store r1 and pc on stack
    push {r1, pc}

    //disable all interrupts (set Interrupt Master Enable to 0)
    mov r1, #0x04000000
    str r1, [r1, #0x208]

    //load and store cpsr on stack
    //GET_CPSR(r1)
    //push {r1}

    //stack snapshot is in main ram
    //r1 contains base address of snapshot
    ldr r1, =__crash_snapshot
    str r0, [r1]
    str r2, [r1, #(2 << 2)]
    str r3, [r1, #(3 << 2)]
    str r4, [r1, #(4 << 2)]
    str r5, [r1, #(5 << 2)]
    str r6, [r1, #(6 << 2)]
    str r7, [r1, #(7 << 2)]
    str r8, [r1, #(8 << 2)]
    str r9, [r1, #(9 << 2)]
    str r10, [r1, #(10 << 2)]
    str r11, [r1, #(11 << 2)]
    str r12, [r1, #(12 << 2)]
    str lr, [r1, #(14 << 2)]

    //pop old cpsr off stack (into r6)
    //pop {r6}

    //pop old value of r1 and pc off stack again (into r4 and r5) and put it in the stack snapshot as well
    pop {r4, r5}
    str r4, [r1, #(1 << 2)]
    str r5, [r1, #(15 << 2)]

    //store sp into stack snapshot now that r1 and pc have been popped again
    str sp, [r1, #(13 << 2)]

    //store old cpsr and spsr into snapshot
    GET_CPSR(r4)
    GET_SPSR(r5)
    add r1, r1, #(16 << 2)
    stmia r1!, {r4, r5}

    mov r10, r0
    mov r11, r1

    //switch to system mode
    mov r0, #0x1F
    SET_CPSR(r0)

    //switch to internal stack
    ldr r0, =switchToInternalStack
    blx r0

    //copy entire dtcm to snapshot
    ldr r4, =fastCopy
    ldr r0, =__dtcm_start
    mov r1, r11
    ldr r2, =__dtcm_size
    blx r4

    //switch back to dtcm stack (old stack will be overwritten, but it doesn't matter since we've already snapshotted it)
    ldr r0, =switchToDTCMStack
    blx r0

    mov r0, r10
    ldr r4, =_crash_doCrash
    bx r4