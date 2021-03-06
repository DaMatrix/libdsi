#include <libdsi/asm.h>

    .arm

    .align 4
ASM_FUNC doMain
    push {r4-r11, lr}
    mov r8, r0
    mov r4, #1
    ldr r5, =fastFillHalfWords
    ldr r6, =0x7FFF
    ldr r7, =_asm_vBlankIntrWait
    //mov r10, #480
    mov r10, #120
_doMain_body:
    eor r4, r4, r4, lsl#13
    eor r4, r4, r4, lsr#17
    eor r4, r4, r4, lsl#5

    and r0, r4, r6
    orr r0, r0, #0x8000
    mov r1, r8
    mov r2, #0x18000 //full screen

    blx r5
    blx r7

    subs r10, r10, #1
    bne _doMain_body
    pop {r4-r11, lr}
    bx lr
