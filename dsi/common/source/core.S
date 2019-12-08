#include <libdsi/asm.h>

    .arm
    .section .itcm

    .align 4
ASM_FUNC copyChunks32
    lsl r2, r2, #5
    add r2, r0, r2 //convert r2 into the final address
    push {r4, r5, r6, r7, r8, r9, r10} //push data registers to stack
copyChunks32_doChunk:
    ldmia r0!, {r3, r4, r5, r6, r7, r8, r9, r10} //read chunk
    stmia r1!, {r3, r4, r5, r6, r7, r8, r9, r10} //write it again
    cmp r0, r2 //check if we're at the final chunk yet
    bne copyChunks32_doChunk
    pop {r4, r5, r6, r7, r8, r9, r10} //restore data registers
    bx lr
