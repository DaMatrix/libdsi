#include <libdsi/asm.h>

#ifdef ARM9
    .section .itcm
#endif

ASM_FUNC stdRootHandler
    mov r12, #0x04000000

    //disable interrupts, or return immedately if interrupts are disabled
    ldr r1, [r12, #0x208]
    cmp r1, #0
    bxeq lr
    mov r0, #0
    str r0, [r12, #0x208]

    //save contents of spsr, old IME,
    mrs	r0, spsr
    push {r0, r1, r12, lr}

    //read IE and IF registers (0x04000210 and 0x04000214, respectively)
    add r1, r12, #0x210
    ldmia r1, {r1, r2}
    ands r1, r1, r2

#ifdef ARM9
    //on ARM9 side, we want to immediately return if no interrupts are ready
    beq _stdRootHandler_wrap_up
#endif

    //load table and flags addresses if the any interrupts are ready
    //these will only be skipped on ARM7 side
	ldrne r0, =__irq_flags
	ldrne r2, =irqTable
#ifdef ARM7
    bne _stdRootHandler_update_flags
#else
	ldrne r0, =__irq_flags
	ldrne r2, =irqTable

_stdRootHandler_update_flags:

_stdRootHandler_wrap_up:

    bx lr
