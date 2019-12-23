#include <libdsi/asm.h>

    .equ _libnds_argv,0x02FFFE70

    .section .crt0, "ax"

    .arm
    .align 4

ASM_FUNC _start
    //disable all interrupts (set Interrupt Master Enable to 0)
    mov r12, #0x04000000
    str r12, [r12, #0x208]

    //switch to internal stack
    ldr r3, =switchToInternalStack
    blx r3

    //
    // setup memory protection
    //

    //TODO: rewrite this for full libnds separation
    ldr r3, =__libnds_mpu_setup
    blx r3

    //switch to system mode
    mov r0, #0x1F
    SET_CPSR(r0)

    //check for current console mode
    ldr r3, =checkDSiMode
    blx r3
    mov r11, r0

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

    //fastCopy and fastClear is now located in itcm, so we can use them instead of bugged BIOS CpuFastSet function
    ldr r4, =fastCopy
    ldr r5, =fastClear

    //vectors (also goes into itcm)
    ldr r0, =__vectors_lma
    ldr r1, =__vectors_start
    ldr r2, =__vectors_end
    sub r2, r2, r1
    blx r4

    //clear dtcm (not really needed per se, but can't hurt and is super fast because everything is in itcm or dtcm)
    ldr r0, =__dtcm_start
    ldr r1, =__dtcm_size
    blx r5

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
    //switch to debug stack because internal stack is in bss, which is about to be cleared by the following code
    ldr r3, =switchToDebugStack
    blx r3

    //clear bss
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    sub r1, r1, r0
    blx r5

    //clear sbss
    ldr r0, =__sbss_start
    ldr r1, =__sbss_end
    sub r1, r1, r0
    blx r5

    //don't clear arm9i bss if not on DSi
    cmp r11, #1
    bne _crt0_init

    //clear arm9i bss
    ldr r0, =__twl_bss_start__
    ldr r1, =__twl_bss_end__
    sub r1, r1, r0
    blx r5

    //cmp r11, #1
    //ldrne r10, =__end__ // (DS mode) heap start
    //ldreq r10, =__twl_end__ // (DSi mode) heap start
    ldr r10, =__twl_end__

    //set DSi mode flag
    //TODO: remove this for libnds separation
    ldr r9, =__dsimode
    strb r11, [r9]

    ldr r0, =_libnds_argv

    //TODO: these symbols are being discarded by ld gc-sections?!?!?!?
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
    //switch back to dtcm stack
    ldr r3, =switchToDTCMStack
    blx r3

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
    ldrne r0, =_arm9_crt0_ntr_errormsg
    ldrne r4, =crashSystem
    blxne r4

    //enable 134mhz clock if in dsi mode
    ldr r0, =enableClockDSi
    blx r0

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

    .section .itcm.enableClockDSi, "ax"
ASM_FUNC enableClockDSi
    //store lr before dsi mode check
    push {lr}

    //check if we are in dsi mode
    ldr r0, =checkDSiMode
    blx r0

    //if not, crash system (shouldn't be possible to get here since we already crash before this is called, but better safe than sorry
    ldrne r0, =_arm9_crt0_clock_errormsg
    ldrne r3, =crashSystem
    blxne r3

    //restore lr
    pop {lr}

    //scfg_clk register
    ldr r0, =0x04004004
    ldrh r1, [r0]
    orr r1, r1, #1

    //we need to wait at least 8 cycles after switching clock speeds
    mov r2, #8

    //actually change speeds
    strh r1, [r0]

    //loop until r2 is 0 (actually this is 16 cycles...)
_enable_twl_clock_loop:
    subs r2, r2, #1
    bne _enable_twl_clock_loop
    
    //return
    bx lr

    //string constants
    .section .rodata
_arm9_crt0_ntr_errormsg:
    .string "Must be run in DSi mode!"
    
_arm9_crt0_clock_errormsg:
    .string "Attempted to set TWL speed in NDS mode!"

    .end
