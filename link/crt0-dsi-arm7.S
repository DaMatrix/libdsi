#include <libdsi/asm.h>

    .section .crt0, "ax"

    .arm
    .align 4

ASM_FUNC _start
    //disable all interrupts (set Interrupt Master Enable to 0)
    mov r0, #0x04000000
    str r0, [r0, #0x208]

    //
    // set stack
    //

    //arm irq mode
    mov r0, #0x12
    msr cpsr, r0
    ldr sp, =__sp_irq

    //arm svc mode
    mov r0, #0x13
    msr cpsr, r0
    ldr sp, =__sp_svc

    //arm system mode
    mov r0, #0x1F
    msr cpsr, r0
    ldr sp, =__sp_usr

    //
    // sync with arm9
    //

    //see sync section in crt0-dsi-arm9.S for description of sync process

    mov r12, #0x4000000
    mov r9, #0x0
    bl IPCSync
    mov r9, #(0x9<<8)
    str r9, [r12, #0x180]
    mov r9, #0xA
    bl IPCSync
    mov r9, #(0xB<<8)
    str r9, [r12, #0x180]
    mov r9, #0xC
    bl IPCSync
    mov r9, #(0xD<<8)
    str r9, [r12, #0x180]
IPCRecvFlag:
    ldr r10, [r12, #0x180]
    and r10, r10, #0xF
    cmp r10, #0xC
    beq IPCRecvFlag

    //copy arm7 code into arm7-wram
    adr r0, arm7lma
    ldr r1, [r0]
    add r0, r1, r0
    ldr r1, =__arm7_start__
    ldr r2, =__arm7_end__
    sub r2, r2, r1
    lsr r2, r2, #2
    swi 0x0C0000 //CpuFastSet

    //clear bss
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    sub r1, r1, r0
    ldr r3, =fastClear
    bl _blx_r3_stub

    cmp r10, #1
    bne NotDSi_prepareCleanup
    //set DSi mode flag
    //TODO: remove this for libnds separation
    ldr r1, =__dsimode
    strb r10, [r1]

    ldr r0, =0x02ffe1d8 // Get ARM7i LMA from header
    ldr r0, [r0]
    ldr r1, =__arm7i_start__
    ldr r2, =__arm7i_end__
    sub r2, r2, r1
    ldr r3, =fastCopy
    bl _blx_r3_stub

    //clear twl bss
    ldr r0, =__twl_bss_start__
    ldr r1, =__twl_bss_end__
    sub r1, r1, r0
    ldr r3, =fastClear
    bl _blx_r3_stub

    //run libc init (static constructors)
    ldr r3, =__libc_init_array // global constructors
    bl _blx_r3_stub

NotDSi_prepareCleanup:
    //finish arm9 sync (arm9 has been spinlocking the whole time)
    mov r9, #0
    mov r12, #0x4000000 // tell arm9 we are ready
    str r9, [r12, #0x180]

    //if not in DSi mode, we can now spinlock forever
    cmp r10, #1
    bne NotDSi_infinilock

    mov r0, #0 //argc
    mov r1, #0 //argv

    //run main method
    ldr r3, =main
    ldr lr, =__libnds_exit
_blx_r3_stub:
    bx r3

arm7lma:
    .word __arm7_lma__ - .
    .pool

IPCSync:
    ldr r10, [r12, #0x180]
    and r10, r10, #0xF
    cmp r10, r9
    bne IPCSync
    bx lr

NotDSi_infinilock:
    b NotDSi_infinilock

    .end
