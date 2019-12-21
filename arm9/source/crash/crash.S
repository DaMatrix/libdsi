#include <libdsi/asm.h>

ASM_FUNC crashSystem
    //store all registers on stack to preserve them
    //stack grows down apparently...
    //stmdb sp, {r0-r12, sp, lr, pc}
    push {r0-r12, sp, lr, pc}

    //save old sp
    //mov r1, sp
    //sub sp, sp, #(16 << 2) //increment sp

    //TODO: copy whole DTCM to some other memory region? (maybe somewhere in main RAM)

    ldr r2, =__sp_usr
    ldr r4, =_crash_doCrash
    bx r4
