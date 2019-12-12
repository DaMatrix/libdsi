/**
 * In order to not require on any devkitpro/ARM things I have to work around the fact that there really is no system, and the libgloss implementation
 * seems to be gone as well.
 *
 * The devkitARM nds implementation of libgloss at https://github.com/devkitPro/newlib/blob/master/libgloss/nds32/ is written in some undecipherable
 * assembly notation.
 *
 * This file contains implementations of all libgloss functions, or stubs.
 */

#include <libdsi/asm.h>

/**
 * _getpid
 */
ASM_FUNC _getpid
    mov r0, #1 //always return 1, dsi doesn't need any processes
    bx lr

/**
 * _exit
 */
ASM_FUNC _exit
    //no-op
    bx lr

/**
 * _kill
 */
ASM_FUNC _kill
    //no-op
    bx lr

/**
 * _sbrk
 */
ASM_FUNC _sbrk
    ldr r3, =heap_end
    ldr r1, [r3] //get current heap end address
    bne __sbrk_try_grow //if heap end is not 0, don't create initial heap
    ldr r1, =__end__ //load end address of program
    add r1, r1, #1024 //initial heap size is 1KiB

__sbrk_try_grow:
    //align r0 (grow by size) to 8-byte boundary
    add r0, r0, #7
    lsr r0, r0, #3
    //lsl r0, r0, #3
    add r0, r1, r0, lsl#3 //r0 is now new heap end address

    ldr r2, =__ram_end
    cmp r0, r2
    bhi __sbrk_error //error if new end would be higher than __ram_end

    str r0, [r3] //store new heap end
    mov r0, r1 //return old heap end address
    bx lr

__sbrk_error:
    mov r0, #12 //ENOMEM
    ldr r1, =_impure_ptr
    str r0, [r1] //set errno
    mov r0, #-1 //-1 return value indicates error
    bx lr

    .data
    .section .bss
    .align 4
heap_end: //this contains the current heap end address, 0 by default
    .word 0
