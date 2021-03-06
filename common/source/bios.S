#include <libdsi/asm.h>

    .thumb
    .align 4

    .section .text._asm_div, "ax"
ASM_FUNC _asm_div
    swi 0x09
    bx lr

    .section .text._asm_sqrt, "ax"
ASM_FUNC _asm_sqrt
    swi 0x0D
    bx lr

    .section .text._asm_bitUnPack, "ax"
ASM_FUNC _asm_bitUnPack
    swi 0x10
    bx lr

// these functions are only present on NDS9/DSi9
#ifdef ARM9
    .section .text._asm_diff8bitUnFilterWrite8bit, "ax"
ASM_FUNC _asm_diff8bitUnFilterWrite8bit
    swi 0x16
    bx lr

    .section .text._asm_diff16bitUnFilter, "ax"
ASM_FUNC _asm_diff16bitUnFilter
    swi 0x18
    bx lr
#endif

    .section .text._asm_huffUnCompReadByCallback, "ax"
ASM_FUNC _asm_huffUnCompReadByCallback
    swi 0x13
    bx lr

    .section .text._asm_lz77UnCompReadNormalWrite8bit, "ax"
ASM_FUNC _asm_lz77UnCompReadNormalWrite8bit
    swi 0x11
    bx lr

    .section .text._asm_lz77UnCompReadByCallbackWrite8bit, "ax"
ASM_FUNC _asm_lz77UnCompReadByCallbackWrite8bit
    swi 0x01
    bx lr

    .section .text._asm_lz77UnCompReadByCallbackWrite16bit, "ax"
ASM_FUNC _asm_lz77UnCompReadByCallbackWrite16bit
    //TODO: figure out whether this is 0x02 or 0x16 (gbatek just says "or")
    swi 0x02
    bx lr

    .section .text._asm_rlUnCompReadNormalWrite8bit, "ax"
ASM_FUNC _asm_rlUnCompReadNormalWrite8bit
    swi 0x14
    bx lr

    .section .text._asm_rlUnCompReadByCallbackWrite16bit, "ax"
ASM_FUNC _asm_rlUnCompReadByCallbackWrite16bit
    swi 0x15
    bx lr

    .section .text._asm_cpuSet, "ax"
ASM_FUNC _asm_cpuSet
    swi 0x0B
    bx lr

    .section .text._asm_cpuFastSet, "ax"
ASM_FUNC _asm_cpuFastSet
    swi 0x0C
    bx lr

    .section .text._asm_halt, "ax"
ASM_FUNC _asm_halt
    swi 0x06
    bx lr

#ifdef ARM7
    .section .text._asm_sleep, "ax"
ASM_FUNC _asm_sleep
    swi 0x07
    bx lr
#endif

//SWI 0x00 (SoftReset) is only for NDS, not DSi

    .section .text._asm_waitByLoop, "ax"
ASM_FUNC _asm_waitByLoop
    swi 0x03
    bx lr

    .section .text._asm_getCRC16, "ax"
ASM_FUNC _asm_getCRC16
    swi 0x0E
    bx lr

#ifdef ARM7
    .section .text._asm_getSineTable, "ax"
ASM_FUNC _asm_getSineTable
    swi 0x1A
    bx lr

    .section .text._asm_getPitchTable, "ax"
ASM_FUNC _asm_getPitchTable
    swi 0x1B
    bx lr

    .section .text._asm_getVolumeTable, "ax"
ASM_FUNC _asm_getVolumeTable
    swi 0x1C
    bx lr

    .section .text._asm_soundBias, "ax"
ASM_FUNC _asm_soundBias
    swi 0x08
    bx lr
#endif

//disabled for now, i don't think this is useful for anything
#if false
#ifdef ARM9
    .section .text._asm_customPost, "ax"
ASM_FUNC _asm_customPost
    swi 0x1F
    bx lr
#endif
#endif

    .section .twl._asm_sha1_init, "ax"
ASM_FUNC _asm_sha1_init
    swi 0x24
    bx lr

    .section .twl._asm_sha1_update, "ax"
ASM_FUNC _asm_sha1_update
    swi 0x25
    bx lr

    .section .twl._asm_sha1_finish, "ax"
ASM_FUNC _asm_sha1_finish
    swi 0x26
    bx lr

    .section .twl._asm_sha1_init_update_finish, "ax"
ASM_FUNC _asm_sha1_init_update_finish
    swi 0x27
    bx lr

    .section .twl._asm_sha1_compare_20_bytes, "ax"
ASM_FUNC _asm_sha1_compare_20_bytes
    swi 0x28
    bx lr

    .section .twl._asm_sha1_init_update_finish_mess, "ax"
ASM_FUNC _asm_sha1_init_update_finish_mess
    swi 0x29
    bx lr

    .section .twl._asm_rsa_init_crypto_heap, "ax"
ASM_FUNC _asm_rsa_init_crypto_heap
    swi 0x20
    bx lr

    .section .twl._asm_rsa_decrypt, "ax"
ASM_FUNC _asm_rsa_decrypt
    swi 0x21
    bx lr

    .section .twl._asm_rsa_decrypt_unpad, "ax"
ASM_FUNC _asm_rsa_decrypt_unpad
    swi 0x22
    bx lr

    .section .twl._asm_rsa_decrypt_unpad_openpgp_sha1, "ax"
ASM_FUNC _asm_rsa_decrypt_unpad_openpgp_sha1
    swi 0x23
    bx lr

    .end
