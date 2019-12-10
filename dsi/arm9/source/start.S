#include <libdsi/asm.h>

    .arm
	.section .crt0

    .align 4
ASM_FUNC _start
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
	//TODO: display some kind of error message if the current console isn't a DSi?

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

	ldr	r1, =__itcm_lma		@ Copy instruction tightly coupled memory (itcm section) from LMA to VMA
	ldr	r2, =__itcm_start
	ldr	r4, =__itcm_end
	bl	CopyMemCheck

	ldr	r1, =__vectors_lma	@ Copy reserved vectors area (itcm section) from LMA to VMA
	ldr	r2, =__vectors_start
	ldr	r4, =__vectors_end
	bl	CopyMemCheck

	ldr	r1, =__dtcm_lma		@ Copy data tightly coupled memory (dtcm section) from LMA to VMA
	ldr	r2, =__dtcm_start
	ldr	r4, =__dtcm_end
	bl	CopyMemCheck

	cmp	r11, #1
	ldrne	r10, =__end__		@ (DS mode) heap start
	ldreq	r10, =__twl_end__	@ (DSi mode) heap start
	bl	checkARGV		@ check and process argv trickery

	ldr	r0, =__bss_start__	@ Clear BSS section
	ldr	r1, =__bss_end__
	sub	r1, r1, r0
	bl	ClearMem

	ldr	r0, =__sbss_start	@ Clear SBSS section
	ldr	r1, =__sbss_end
	sub	r1, r1, r0
	bl	ClearMem

	cmp	r11, #1
	bne	NotTWL
	ldr	r9, =__dsimode		@ set DSi mode flag
	strb	r11, [r9]

	@ Copy TWL area (arm9i section) from LMA to VMA
	ldr	r1, =0x02ffe1c8		@ Get ARM9i LMA from header
	ldr	r1, [r1]

	ldr	r2, =__arm9i_start__
	cmp	r1, r2			@ skip copy if LMA=VMA
	ldrne	r4, =__arm9i_end__
	blne	CopyMemCheck

	ldr	r0, =__twl_bss_start__	@ Clear TWL BSS section
	ldr	r1, =__twl_bss_end__
	sub	r1, r1, r0
	bl	ClearMem

NotTWL:
	ldr	r0, =_libnds_argv

	@ reset heap base
	ldr	r2, [r0,#20]		@ newheap base
	cmp	r2, #0
	moveq	r2, r10
	ldr	r1, =fake_heap_start	@ set heap start
	str	r2, [r1]

	ldr	r1, =fake_heap_end	@ set heap end
	sub	r8, r8,#0xc000
	str	r8, [r1]

	push	{r0}
	ldr	r0, =__secure_area__
	ldr	r3, =initSystem
	blx	r3			@ system initialisation

	ldr	r3, =__libc_init_array	@ global constructors
	blx	r3

	pop	{r0}

	ldr	r1, [r0,#16]		@ argv
	ldr	r0, [r0,#12]		@ argc

	ldr	r3, =main
	ldr	lr, =__libnds_exit
	bx	r3			@ jump to user code

@---------------------------------------------------------------------------------
@ check for a commandline
@---------------------------------------------------------------------------------
checkARGV:
@---------------------------------------------------------------------------------
	ldr	r0, =_libnds_argv	@ argv structure
	mov	r1, #0
	str	r1, [r0,#12]		@ clear argc
	str	r1, [r0,#16]		@ clear argv

	ldr	r3, [r0]		@ argv magic number
	ldr	r2, =0x5f617267		@ '_arg'
	cmp	r3, r2
	strne	r1, [r0,#20]
	bxne	lr			@ bail out if no magic

	ldr	r1, [r0, #4]		@ command line address
	ldr	r2, [r0, #8]		@ length of command line

	@ copy to heap
	mov	r3, r10			@ initial heap base
	str	r3, [r0, #4]		@ set command line address

	cmp	r2, #0
	subnes	r4, r3, r1		@ dst-src
	bxeq	lr			@ dst == src || len==0 : nothing to do.

	cmphi	r2, r4			@ len > (dst-src)
	bhi	.copybackward

.copyforward:
	ldrb	r4, [r1], #1
	strb	r4, [r3], #1
	subs	r2, r2, #1
	bne	.copyforward
	b	.copydone

.copybackward:
	subs	r2, r2, #1
	ldrb	r4, [r1, r2]
	strb	r4, [r3, r2]
	bne	.copybackward

.copydone:
	push	{lr}
	ldr	r3, =build_argv
	blx	r3
	pop	{lr}
	bx	lr
