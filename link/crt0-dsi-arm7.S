#include <libdsi/asm.h>

    .section .crt0, "ax"

    .arm
    .align 4

ASM_FUNC _start
    //disable all interrupts (set Interrupt Master Enable to 0)
    mov r12, #0x04000000
    str r12, [r12, #0x208]

    //
    // set stack
    //

    bl _configure_stack

    //switch to system mode
    mov r0, #0x1F
    msr cpsr, r0

    //
    // sync with arm9
    //

    //see sync section in crt0-dsi-arm9.S for description of sync process

    //copy sync code into arm7-wram before executing it
    //i'm not sure why this is required (maybe too much contention on the main memory bus?), but sync code never finishes without it
    adr r0, __sync_start
    ldr r1, =__arm7_start__
    mov r2, #(__sync_end-__sync_start)
    lsr r2, r2, #2
    mov r4, r1
    swi 0x0C0000 //CpuFastSet

    //acutally do arm9 sync
    bl _blx_r4_stub

    //
    // copy code into correct memory regions and clear bss sections
    //

    //copy arm7 code into arm7-wram
    adr r0, arm7lma
    ldr r1, [r0]
    add r0, r1, r0
    ldr r1, =__arm7_start__
    ldr r2, =__arm7_end__
    sub r2, r2, r1
    lsr r2, r2, #2
    swi 0x0C0000 //CpuFastSet

    //don't copy arm7i stuff if not on DSi
    cmp r10, #1
    bne _crt0_bss

    //arm7i code+data
    ldr r0, =0x02FFE1D8
    ldr r0, [r0]
    ldr r1, =__arm7i_start__
    ldr r2, =__arm7i_end__
    sub r2, r2, r1
    bl fastCopy

_crt0_bss:
    //clear bss
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    sub r1, r1, r0
    bl fastClear

    //don't clear arm7i bss if not on DSi
    cmp r10, #1
    bne _crt0_init

    //clear arm7i bss
    ldr r0, =__twl_bss_start__
    ldr r1, =__twl_bss_end__
    sub r1, r1, r0
    bl fastClear

_crt0_init:
    //set DSi mode flag
    //TODO: remove this for libnds separation
    ldr r1, =__dsimode
    strb r10, [r1]

    //run libc init (static constructors)
    ldr r4, =__libc_init_array
    bl _blx_r4_stub

    //finish arm9 sync (arm9 has been spinlocking the whole time)
    mov r12, #0x4000000
    str r0, [r12, #0x180]

    //if not in DSi mode, we can now just wait forever
    cmp r10, #1
    bne NotDSi_infinite_wait

    //run system init
    bl resetSystem

    //set r0 (argc) to 0 and r1 (argv) to nullptr
    mov r0, #0
    mov r1, #0

    //main method should link back to __libnds_exit
    //TODO: use my own exit handler for libnds separation
    ldr lr, =__libnds_exit

    //branch to main method, crt0 is now complete
    ldr r4, =main

_blx_r4_stub:
    bx r4

arm7lma:
    .word __arm7_lma__ - .
    .pool

__sync_start:
    push {lr}
    mov r9, #0x0
    bl _arm9_sync_loop
    mov r9, #(0x9<<8)
    str r9, [r12, #0x180]
    mov r9, #0xA
    bl _arm9_sync_loop
    mov r9, #(0xB<<8)
    str r9, [r12, #0x180]
    mov r9, #0xC
    bl _arm9_sync_loop
    mov r9, #(0xD<<8)
    str r9, [r12, #0x180]
_arm9_sync_recv_dsi_flag:
    ldr r10, [r12, #0x180]
    and r10, r10, #0xF
    cmp r10, #0xC
    beq _arm9_sync_recv_dsi_flag
    pop {pc}

_arm9_sync_loop:
    ldr r10, [r12, #0x180]
    and r10, r10, #0xF
    cmp r10, r9
    bne _arm9_sync_loop
    bx lr
__sync_end:

NotDSi_infinite_wait:
    ldr r4, =_asm_vBlankIntrWait
_NotDSi_do_infinite_wait:
    bl _blx_r4_stub
    b _NotDSi_do_infinite_wait

ASM_FUNC _configure_stack
    //save original cpsr
    GET_CPSR(r1)

    //arm interrupt mode
    mov r0, #0x12
    msr cpsr, r0
    ldr sp, =__sp_irq

    //arm supervisor mode
    mov r0, #0x13
    msr cpsr, r0
    ldr sp, =__sp_svc

    //arm system mode
    mov r0, #0x1F
    msr cpsr, r0
    ldr sp, =__sp_usr

    //restore original cpsr
    SET_CPSR(r1)

    //return
    bx lr

    .end
