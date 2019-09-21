#include <libdsi/asm.h>

    .thumb

ASM_FUNC _asm_div
    swi 0x09
    bx lr

ASM_FUNC _asm_sqrt
    swi 0x0D
    bx lr

ASM_FUNC _asm_bitUnPack
    swi 0x10
    bx lr

// these functions are only present on NDS9/DSi9
#ifdef ARM9
ASM_FUNC _asm_diff8bitUnFilterWrite8bit
    swi 0x16
    bx lr

ASM_FUNC _asm_diff16bitUnFilter
    swi 0x18
    bx lr
#endif

ASM_FUNC _asm_huffUnCompReadByCallback
    swi 0x13
    bx lr

ASM_FUNC _asm_lz77UnCompReadNormalWrite8bit
    swi 0x11
    bx lr

ASM_FUNC _asm_lz77UnCompReadByCallbackWrite8bit
    swi 0x01
    bx lr

ASM_FUNC _asm_lz77UnCompReadByCallbackWrite16bit
    //TODO: figure out whether this is 0x02 or 0x16 (gbatek just says "or")
    swi 0x02
    bx lr

ASM_FUNC _asm_rlUnCompReadNormalWrite8bit
    swi 0x14
    bx lr

ASM_FUNC _asm_rlUnCompReadByCallbackWrite16bit
    swi 0x15
    bx lr

ASM_FUNC _asm_cpuSet
    swi 0x0B
    bx lr

ASM_FUNC _asm_cpuFastSet
    swi 0x0C
    bx lr

ASM_FUNC _asm_halt
    swi 0x06
    bx lr

ASM_FUNC _asm_intrWait
    swi 0x04
    bx lr

ASM_FUNC _asm_vBlankIntrWait
    swi 0x05
    bx lr

#ifdef ARM7
ASM_FUNC _asm_sleep
    swi 0x07
    bx lr
#endif

//SWI 0x00 (SoftReset) is only for NDS, not DSi

ASM_FUNC _asm_waitByLoop
    swi 0x03
    bx lr

ASM_FUNC _asm_getCRC16
    swi 0x0E
    bx lr

#ifdef ARM7
ASM_FUNC _asm_getSineTable
    swi 0x1A
    bx lr

ASM_FUNC _asm_getPitchTable
    swi 0x1B
    bx lr

ASM_FUNC _asm_getVolumeTable
    swi 0x1C
    bx lr

ASM_FUNC _asm_soundBias
    swi 0x08
    bx lr
#endif

//disabled for now, i don't think this is useful for anything
#if false
#ifdef ARM9
ASM_FUNC _asm_customPost
    swi 0x1F
    bx lr
#endif
#endif

ASM_FUNC _asm_sha1_init
    swi 0x24
    bx lr

ASM_FUNC _asm_sha1_update
    swi 0x25
    bx lr

ASM_FUNC _asm_sha1_finish
    swi 0x26
    bx lr

ASM_FUNC _asm_sha1_init_update_finish
    swi 0x27
    bx lr

ASM_FUNC _asm_sha1_compare_20_bytes
    swi 0x28
    bx lr

ASM_FUNC _asm_sha1_init_update_finish_mess
    swi 0x29
    bx lr

ASM_FUNC _asm_rsa_init_crypto_heap
    swi 0x20
    bx lr

ASM_FUNC _asm_rsa_decrypt
    swi 0x21
    bx lr

ASM_FUNC _asm_rsa_decrypt_unpad
    swi 0x22
    bx lr

ASM_FUNC _asm_rsa_decrypt_unpad_openpgp_sha1
    swi 0x23
    bx lr
