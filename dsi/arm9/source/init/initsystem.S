#include <libdsi/asm.h>

ASM_FUNC initSystem
    mov r0, #0 //r0: counter
    mov r1, #0x4000000
    orr r1, r1, #0xB0 //r1: dma channel register address
    mov r2, #0 //r2: zero
_initSystem_clrDMA:
    str r2, [r1, #8] //clear size and control registers
    str r2, [r1] //clear src register
    str r2, [r1, #4] //clear dst register
    add r1, r1, #12 //move r1 to base address of next DMA channel
    add r0, r0, #1
    cmp r0, #4