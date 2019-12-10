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

    .text

ASM_FUNC _getpid
    mov r0, #1
    bx lr

    .extern	_impure_ptr

ASM_FUNC _exit
ASM_FUNC _kill
    bx lr

ASM_FUNC _sbrk
heap_end:
    .word	0
