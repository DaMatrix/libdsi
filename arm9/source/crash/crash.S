#include <libdsi/asm.h>

ASM_FUNC crashSystemNoMsg
    mov r0, #0

ASM_FUNC crashSystem
    //store r1 and pc on stack
    push {r1, pc}

    //load and store cpsr on stack
    //GET_CPSR(r1)
    //push {r1}

    //check if __crash_isCrashing is set
    //can probably only happen if a bug occurs internally while causing a crash
    //ldr r1, =__crash_isCrashing
    //ldrb r1, [r1]
    //cmp r1, #0
    //bne _crashSystem_recursive_error

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

    //switch to internal stack
    ldr r0, =_switch_to_internal_stack
    blx r0

    //copy entire DTCM to snapshot
    ldr r4, =fastCopy
    ldr r0, =__dtcm_start
    //r1 is already the target address
    ldr r2, =__dtcm_size
    blx r4

    //ldr r2, =__sp_usr
    mov r1, sp
    ldr r2, =__internal_dummy_stack
    ldr r4, =_crash_doCrash
    bx r4

_crashSystem_recursive_error:
    //__crash_isCrashing was set! something is horribly wrong...
    ldr r1, =_asm_vBlankIntrWait

    //link back to top of function for infinite loop lol
    mov lr, r1
    bx r1
