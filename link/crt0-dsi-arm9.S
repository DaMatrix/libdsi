#include <libdsi/asm.h>

    .equ _libnds_argv,0x02FFFE70

    .section .crt0, "ax"

    .arm
    .align 4

ASM_FUNC _start
    //disable all interrupts (set Interrupt Master Enable to 0)
    mov r0, #0x04000000
    str r0, [r0, #0x208]

    //
    // set temporary stack
    //

    //arm svc mode
    mov r0, #0x13
    msr cpsr, r0
    mov r1, #0x03000000
    sub r1, r1, #0x1000
    mov sp, r1

    //arm system mode
    mov r0, #0x1F
    msr cpsr, r0
    sub r1, r1, #0x100
    mov sp, r1

    //
    // setup memory protection
    //

    //TODO: rewrite this for full libnds separation
    ldr r3, =__libnds_mpu_setup
    blx r3

    //
    // set actual stack
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

    //check for current console mode
    mov r12, #0x4000000
    ldrb r11, [r12, r12, lsr#12]
    and r11, r11, #0x3
    cmp r11, #1
    movne r11, #0 //only accept DSi mode, everything else is invalid

    //
    // ARM7 sync
    //

    //libnds sync process:
    //ARM9 sends 0x0
    //ARM7 replies with 0x9
    //ARM9 replies with 0xA
    //ARM7 replies with 0xB
    //ARM9 replies with 0xC
    //ARM7 replies with 0xD
    //ARM9 replies with current DSi mode flag
    //ARM7 copies code into WRAM from main RAM and then replies with 0x0, sync is completed

    //TODO: redo this
    mov r9, #(0x0 << 8)
    str r9, [r12, #0x180]
    mov r9, #0x9
    bl IPCSync
    mov r9, #(0xA << 8)
    str r9, [r12, #0x180]
    mov r9, #0xB
    bl IPCSync
    mov r9, #(0xC << 8)
    str r9, [r12, #0x180]
    mov r9, #0xD
    bl IPCSync
    mov r9, r11, lsl #8
    str r9, [r12, #0x180]
    mov r9, #0
    bl IPCSync
    str r9, [r12, #0x180]

    //
    // move memory regions to their actual positions
    //

    //itcm code
    ldr r0, =__itcm_lma
    ldr r1, =__itcm_start
    ldr r2, =__itcm_end
    sub r2, r2, r1
    lsr r2, r2, #2
    swi 0x0C0000 //CpuFastSet

    ldr r4, =fastCopy //fastCopy is now located in ITCM, so we can use it instead of bugged BIOS CpuFastSet function

    //vectors (also goes into itcm)
    ldr r0, =__vectors_lma
    ldr r1, =__vectors_start
    ldr r2, =__vectors_end
    sub r2, r2, r1
    blx r4

    //dtcm
    //TODO: is this even needed? DTCM should really only be used for stack
    ldr r0, =__dtcm_lma
    ldr r1, =__dtcm_start
    ldr r2, =__dtcm_end
    sub r2, r2, r1
    blx r4

    //don't copy arm9i stuff if not on DSi
    cmp r11, #1
    bne _crt0_bss

    //arm9i code+data
    ldr r0, =0x02ffe1c8
    ldr r0, [r0]
    ldr r1, =__arm9i_start__
    ldr r2, =__arm9i_end__
    sub r2, r2, r1
    blx r4

_crt0_bss:
    ldr r4, =fastClear

    //clear bss
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    sub r1, r1, r0
    blx r4

    //clear sbss
    ldr r0, =__sbss_start
    ldr r1, =__sbss_end
    sub r1, r1, r0
    blx r4

    //don't clear arm9i bss if not on DSi
    cmp r11, #1
    bne _crt0_init

    //clear arm9i bss
    ldr r0, =__twl_bss_start__
    ldr r1, =__twl_bss_end__
    sub r1, r1, r0
    blx r4

    //cmp r11, #1
    //ldrne r10, =__end__ // (DS mode) heap start
    //ldreq r10, =__twl_end__ // (DSi mode) heap start
    ldr r10, =__twl_end__

    //set DSi mode flag
    //TODO: remove this for libnds separation
    ldr r9, =__dsimode
    strb r11, [r9]

    ldr r0, =_libnds_argv

    // reset heap base
    ldr r2, [r0, #20] // newheap base
    cmp r2, #0
    moveq r2, r10
    ldr r1, =fake_heap_start // set heap start
    str r2, [r1]

    ldr r1, =fake_heap_end // set heap end
    sub r8, r8, #0xc000
    str r8, [r1]

_crt0_init:
    //load secure area address
    ldr r0, =__secure_area__ //TODO: i can set this to 0 without causing issues, figure out why actually deleting it or the line in securearea.c prevents the ROM from loading
    mov r0, #0

    //run system init
    ldr r3, =initSystem
    blx r3

    //run libc init (static constructors)
    ldr r3, =__libc_init_array
    blx r3

    //display error message if not running on DSi
    cmp r11, #1
    bne NotDSi_error

    bl enable_twl_clock

    mov r0, #0 //argc
    mov r1, #0 //argv

    //run main method
    ldr r3, =main
    ldr lr, =__libnds_exit
    bx r3

IPCSync:
    ldr r10, [r12, #0x180]
    and r10, r10, #0xF
    cmp r10, r9
    bne IPCSync
    bx lr

NotDSi_error:
    ldr r0, =NotDSi_errormsg
    ldr r4, =crashSystem
    blx r4

    .section .itcm
enable_twl_clock:
    //avoid breaking stuff?!?
    ldr r0, =__dsimode
    cmp r0, #1
    ldrne r0, =NotDSi_clock_errormsg
    ldrne r4, =crashSystem
    blxne r4

    //SCFG_CLK
    ldr r0, =0x04004004
    ldrh r1, [r0]
    orr r1, r1, #1
    mov r2, #8 //cycles to wait
    strh r1, [r0]

_enable_twl_clock_loop:
    subs r2, r2, #1
    bne _enable_twl_clock_loop
    bx lr

    .section .rodata
NotDSi_errormsg:
    .string "Must be run in DSi mode!"
NotDSi_clock_errormsg:
    .string "Attempted to set TWL speed in NDS mode!"

    .end
