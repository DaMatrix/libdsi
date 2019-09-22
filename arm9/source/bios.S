#include <libdsi/asm.h>

    .arm

ASM_FUNC _asm_vBlankIntrWait
    mov r0, #1
    mov r1, #1

//TODO: this is currently just a copypasta of the libnds implementation, i can definitely make this Better:tm: and Faster:tm:
ASM_FUNC _asm_intrWait
    push {lr}
    mov r12, #0x4000000
    str r12, [r12, #0x208]

    mrc p15, 0, r3, c9, c1, 0
    mov r3, r3, lsr#12
    mov r3, r3, lsl#12
    add r3, r3, #0x4000

    mov r12, r0
    bl check_flags
    beq wait

    cmp r12, #0
    beq flag_set

wait:
    mov r12, #0x4000000

wait_flags:
    mov r2, #1
    str r2, [r12,#0x208]

    mov r0, #0
    mcr p15, 0, r0, c7, c0, 4

    str r12, [r12, #0x208]
    bl check_flags
    beq wait_flags

flag_set:
    mov r2, #1
    str r2, [r12,#0x208]
    pop {pc}

check_flags:
    ldr r2, [r3, #-8]
    ands r0, r1, r2
    eorne r2, r2, r0
    strne r2, [r3, #-8]
    bx lr
