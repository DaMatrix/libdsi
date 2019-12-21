#include <libdsi/asm.h>

ASM_FUNC crashSystem
    //store r1 and pc on stack
    push {r1, pc}

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

    //store cpsr and spsr into snapshot
    LOAD_CPSR(r4)
    LOAD_SPSR(r5)
    add r1, r1, #(16 << 2)
    stmia r1!, {r4, r5}

    //TODO: add rest of stack to snapshot

    //ldr r2, =__sp_usr
    ldr r4, =_crash_doCrash
    bx r4
