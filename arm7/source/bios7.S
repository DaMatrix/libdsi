#include <libdsi/asm.h>

    .arm

ASM_FUNC _asm_vBlankIntrWait
    mov r0, #1
    mov r1, #1
    mov r2, #0

//TODO: this is currently just a copypasta of the libnds implementation, i can definitely make this Better:tm: and Faster:tm:
ASM_FUNC _asm_intrWait
    stmfd sp!, {lr}
    cmp r0, #0
    blne testirq

wait_irq:
    swi #(6<<16)
    bl testirq
    beq wait_irq
    ldmfd sp!, {lr}
    bx lr

testirq:
    mov r12, #0x4000000
    strb r12, [r12,#0x208]
    ldr r3, [r12,#-8]
    ands r0, r1, r3
    eorne r3, r3, r0
    strne r3, [r12,#-8]
    mov r0, #1
    strb r0, [r12,#0x208]
    bx lr

    .end
