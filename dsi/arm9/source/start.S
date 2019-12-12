#include <libdsi/asm.h>

    .arm
	//.section .crt0
	.section .text

    .align 4
ASM_FUNC _init
    bx lr

ASM_FUNC _dsi_start
    //set lsb of Interrupt Master Enable (0x04000208) to 0 to disable all interrupts
	mov	r0, #0x04000000
	str	r0, [r0, #0x208]

    //
    // set up stacks
    //

    //irq mode
	mov	r0, #0x12
	msr	cpsr, r0
	ldr	sp, =__sp_irq

    //svc mode
	mov	r0, #0x13
	msr	cpsr, r0
	ldr	sp, =__sp_svc

	//system (user) mode
	mov	r0, #0x1F
	msr	cpsr, r0
	ldr	sp, =__sp_usr

	//get current console mode
	mov	r12, #0x4000000
	ldrb r11, [r12,r12,lsr #12]
	and	r11, r11, #0x3

	//debug: go straight to main method
    mov r0, #0 //argc
    mov r1, #0 //argv

	ldr lr, =shutdown
	ldr	r0, =main
	blx r0 //call main function

	cmp	r11, #1
	bne	NotDSi

	//
	// configure memory protection unit
	//

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

    //
    // arm7 sync
    //

    //get current value of remote IPCSYNC register
    ldr r2, [r12, #0x180]
    and r2, r2, #0xF
    eor r2, r2, #0xF

    //set IPCSYNC register to A
    mov r3, #0x0A00
    str r3, [r12, #0x180]

    //spinlock until remote IPCSYNC register value changes to the eor'd value
arm7_sync_lock:
    ldr r0, [r12, #0x180]
    and r0, r0, #0xF
    eor r0, r0, #0xF
    cmp r0, r2
    bne arm7_sync_lock

    //copy code into actual ITCM
    ldr r0, =__itcm_base
    ldr r1, =__itcm_start
    ldr r2, =__itcm_end
    bl SimpleCopy

    //copy data into actual DTCM
    //TODO: could probably be removed, DTCM is only used as stack
	ldr	r0, =__dtcm_base
	ldr	r1, =__dtcm_start
	ldr	r2, =__dtcm_end
	bl SimpleCopy

	cmp	r11, #1

	//clear BSS region
	ldr	r0, =__bss_start__
	ldr	r1, =__bss_end__
	bl SimpleClear

    //clear SBSS region
	ldr	r0, =__sbss_start
	ldr	r1, =__sbss_end
	bl SimpleClear

	//ldr	r0, =initSystem
	//blx	r0

	ldr	r0, =__libc_init_array
	blx	r0

    mov r0, #0 //argc
    mov r1, #0 //argv

	ldr	r2, =main
	blx r2 //call main function

shutdown:
	//shut down console
	ldr r0, =0x4000304
	ldr r1, [r0]
	orr r1, #(1 << 6)
	str r1, [r0]
	b shutdown //spinlock if for whatever reason this doesn't work

SimpleCopy:
    push {r5-r11}
_SimpleCopy_main:
    ldmia r0!, {r5-r12}
    stmia r1!, {r5-r12}
    cmp r0, r2
    blt _SimpleCopy_main
    pop {r5-r11}
    bx lr

SimpleClear:
    mov r2, #0
_SimpleClear_main:
    stmia r0!, {r2}
    cmp r0, r1
    blt _SimpleClear_main
    bx lr

NotDSi:
    //TODO: display some kind of error message, for now this just spinlocks forever
    b NotDSi
