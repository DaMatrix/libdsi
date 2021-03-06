#include <libdsi/asm.h>

    .arm
    .align 4

#ifdef ARM9
    .section .itcm.stdRootHandler, "ax"
#endif

//this took me forever to figure out, eventually learned by reading nocash's magic floor source code that the bios interrupt handler seems to do quite a
//bit (such as pushing r0-r3 to stack) before calling the user interrupt handler
//see MAGICFLR.A22 lines 907-945
ASM_FUNC stdRootHandler
    //register base
    mov r0, #0x04000000

    //load IME into r0
    ldr r1, [r0, #0x208]

    //check if IME lsb is set, and, if somehow it isn't, immediately return from interrupt handler
    ands r1, r1, #1
    moveq pc, lr

    //disable IME
    mov r1, #0
    str r1, [r0, #0x208]

    //read IE and IF registers (0x04000210 and 0x04000214, respectively)
    add r3, r0, #0x210
    ldmia r3!, {r1, r2}
    ands r1, r1, r2

#ifdef ARM9
    //on ARM9 side, we want to immediately return if no interrupts are ready
    beq _stdRootHandler_wrap_up
#endif

    //load table and flags addresses if any interrupts are ready
    //these will only be skipped on ARM7 side
    ldrne r0, =__irq_flags
    ldrne r2, =__interruptHandlers

#ifdef ARM7
    //on ARM7 side, we want to check for aux irq flags (and skip them if a normal IRQ is already found)
    bne _stdRootHandler_update_flags

    //read IE2 and IF2 registers (0x04000218 and 0x0400021C, respectively)
    //r3 has already been incremented by 8 due to write-back mode on ldmia when reading IE and IF
    ldmia r3!, {r1, r2}
    ands r1, r1, r2

    //immediately return if no aux interrupts are ready either
    beq _stdRootHandler_wrap_up

    //load table and flags addresses if any aux interrupts are ready
    ldr r0, =__irq_flagsaux
    ldr r2, =__interruptHandlers7
#endif

_stdRootHandler_update_flags:
    push {r3}
    //from gbatek:

    //DTCM+3FF8h - NDS9 - IRQ Check Bits (hardcoded DTCM address)
    //380FFF8h - NDS7 - IRQ Check Bits (hardcoded RAM address)
    //
    // Bit 0-31  IRQ Flags (same format as IE/IF registers)
    //
    //When processing & acknowleding interrupts via IF register, the user interrupt handler should also set the corresponding bits of the IRQ Check
    //value (required for BIOS IntrWait and VBlankIntrWait SWI functions).
    ldr r3, [r0]
    orr r3, r3, r1
    str r3, [r0]

    mov r12, #1

    //find bit offset of pending interrupt
#ifdef ARM9
    //fast version for ARM9, counts the number of leading zeroes in r1 to find a waiting interrupt in a single cycle
    //r1 cannot be 0, so clz will always return a value between 0-31 (not 32)
    clz r0, r1

    //invert value
    eor r0, r0, #0x1F
#else
    //(much) slower version for ARM7 (since CLZ only exists on ARM9), simply iterates from 0-31 and checks for non-zero bits in r1
    mov r0, #32
_stdRootHandler_find_handler_loop:
    sub r0, r0, #1

    //check if r1 & (1 << r0) is non-zero (r3 is garbage, r12 is a dummy register for 1)
    ands r3, r1, r12, lsl r0

    //if it's 0, continue loop (r0 will be decremented) //loop cannot be infinite since r1 cannot be 0
    beq _stdRootHandler_find_handler_loop
#endif

    //write bit to IF to clear it
    pop {r3}
    lsl r1, r12, r0
    str r1, [r3, #-4]

    //load interrupt handler address into r2 (effectively r2[r0])
    ldr r0, [r2, r0, lsl #2]

    //skip if handler is nullptr
    cmp r0, #0
    beq _stdRootHandler_wrap_up

    //back up lr
    push {lr}

#ifdef ARM9
    blx r0
#else
    bl _stdRootHandler_arm7_blx_r0
#endif

    //restore lr
    pop {lr}

_stdRootHandler_wrap_up:
    //register base
    mov r0, #0x04000000

    //set IME to 1
    //we can't have gotten this far if IME was 0 before, so there you have it
    //if anyone is trying to be clever by storing extra info in bits 1-31 of IME they can HECK off
    mov r1, #1
    str r1, [r0, #0x208]

    //return lol
    mov pc, lr

#ifdef ARM7
_stdRootHandler_arm7_blx_r0:
    bx r0
#endif
