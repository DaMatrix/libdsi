#include <libdsi/asm.h>

	.section .crt0
    .arm
    .align 4

ASM_FUNC _init
    bx lr

ASM_FUNC _dsi_start
    //debug: spinlock forever
    b _dsi_start

	mov	r0, #0x04000000		@ IME = 0;
	mov	r1, #0
	str	r1, [r0, #0x208]

	mov	r0, #0x12		@ Switch to IRQ Mode
	msr	cpsr, r0
	ldr	sp, =__sp_irq		@ Set IRQ stack

	mov	r0, #0x13		@ Switch to SVC Mode
	msr	cpsr, r0
	ldr	sp, =__sp_svc		@ Set SVC stack

	mov	r0, #0x1F		@ Switch to System Mode
	msr	cpsr, r0
	ldr	sp, =__sp_usr		@ Set user stack

	ldr	r3, =__libc_init_array	@ global constructors
	bl	_blx_r3_stub

    mov r0, #0 //argc
    mov r1, #0 //argv
	ldr	lr, =shutdown
	ldr	r3, =main

_blx_r3_stub:
	bx	r3

shutdown:
	b shutdown