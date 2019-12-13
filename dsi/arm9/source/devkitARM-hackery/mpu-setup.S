#include <libdsi/asm.h>

ASM_FUNC __libnds_mpu_setup
    //display enable + 2d engines A+B
    ldr r0, =0x0203
    mov r1, #0x04000000
    add r1, r1, #0x304
    strh r0, [r1]

    //disable TCM and PU
    ldr r0, =0x00002078
    mcr p15, 0, r0, c1, c0

    //flush caches
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 0 //Invalidate Entire Instruction Cache
    mcr p15, 0, r0, c7, c6, 0 //Invalidate Entire Data Cache
    mcr p15, 0, r0, c7, c10, 4 //flush write buffer

    //set DTCM size/base
    ldr r0, =__dtcm_start
    add r0, r0, #0x0A //dtcm size (512 shl 5, lsb is reserved so A becomes 5)
    mcr p15, 0, r0, c9, c1, 0

    //set ITCM size/base
    mov r0, #0x0C //itcm size (512 shl 6, see DTCM)
    mcr p15, 0, r0, c9, c1, 1

    //setup protection unit in a way similar to https://problemkaputt.de/gbatek.htm#dsmemorycontrolcacheandtcm

    //region 0: IO registers and VRAM
    ldr r0, =0x04000033
    mcr p15, 0, r0, c6, c0, 0

    //region 1: main memory
    ldr r0, =0x02000018
    mcr p15, 0, r0, c6, c1, 0

    //region 2: BIOS
    ldr r0, =0xFFFF001D
    mcr p15, 0, r0, c6, c2, 0

    //region 3: DTCM
    ldr r0, =__dtcm_start
    orr r0, r0, #0x1B
    mcr p15, 0, r0, c6, c3, 0

    //region 4: ITCM
    ldr r0, =__itcm_start
    orr r0, r0, #0x1D
    mcr p15, 0, r0, c6, c4, 0

    //region 5: DSi-WRAM
    ldr r0, =0x0300002D
    mcr p15, 0, r0, c6, c5, 0

    //region 6: non-cached main RAM mirror
    ldr r0, =0x0C00002F
    mcr p15, 0, r0, c6, c6, 0

    //region 7: unused?
    mov r0, #0
    mcr p15, 0, r0, c6, c7, 0

    //enable write buffer
    mov r0, #0b00000010 //only allow write-buffering for main RAM
    mcr p15, 0, r0, c3, c0, 0

    //enable L1 cache
    mov r0, #0b00000010 //only allow caching for main RAM
    mcr p15, 0, r0, c2, c0, 0 //data cache
    mcr p15, 0, r0, c2, c0, 1 //instruction cache

    //set read/write permissions for different regions
    ldr r0, =0x03333333
    mcr p15, 0, r0, c5, c0, 2 //data access
    mcr p15, 0, r0, c5, c0, 3 //instruction access

    //enable cache and TCM
    ldr r1, =0x51005
    mrc p15, 0, r0, c1, c0, 0
    orr r0, r0, r1
    mcr p15, 0, r0, c1, c0, 0

    bx lr
