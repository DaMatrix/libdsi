#include <libdsi/asm.h>

    .thumb

ASM_FUNC _asm_div
    swi 0x09
    bx lr

ASM_FUNC _asm_sqrt
    swi 0x08
    bx lr

ASM_FUNC _asm_bitUnPack
    swi 0x10
    bx lr
