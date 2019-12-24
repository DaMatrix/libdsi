#include <libdsi/asm.h>

ASM_FUNC crashSystemNoMsg
    mov r0, #0

ASM_FUNC crashSystem
    //store r1 and pc on stack
    push {r1, pc}

    //disable all interrupts (set Interrupt Master Enable to 0)
    mov r1, #0x04000000
    str r1, [r1, #0x208]

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

    //pop old value of r1 and pc off stack again (into r4 and r5) and put it in the stack snapshot as well
    pop {r4, r5}
    str r4, [r1, #(1 << 2)]
    str r5, [r1, #(15 << 2)]

    //store sp into stack snapshot now that r1 and pc have been popped again
    str sp, [r1, #(13 << 2)]

    //get old cpsr and spsr
    GET_CPSR(r4)
    GET_SPSR(r5)

    //set T bit of old cpsr based on lr
    and r3, lr, #1
    orr r4, r4, r3, lsl #5

    //store old cpsr and spsr into snapshot
    add r1, r1, #(16 << 2)
    stmia r1!, {r4, r5}

    //move r0 and r1 to r10 and r11 to prevent them from being destroyed by switchToInternalStack and fastCopy afterwards
    mov r10, r0
    mov r11, r1

    //switch to system mode
    mov r0, #0x1F
    SET_CPSR(r0)

    //switch to internal stack
    bl switchToInternalStack

    //copy entire dtcm to snapshot
    ldr r0, =__dtcm_start
    mov r1, r11 //restore old r1 from r11 (from before we changed stacks)
    ldr r2, =__dtcm_size
    bl fastCopy

    //write user message to snapshot
    str r10, [r1], #4

    mrc p15, 0, r10, c1, c0, 0
    str r10, [r1], #4

    //switch back to dtcm stack (old stack will be overwritten, but it doesn't matter since we've already snapshotted it)
    bl switchToDTCMStack

    //display crash screen
    ldr lr, =_crash_doCrash
    ldr r4, =resetSystem
    bx r4
