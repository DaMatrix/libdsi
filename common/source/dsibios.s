    .text
    .thumb
    .global _asm_div
_asm_div:
    swi 0x09
    bx lr
