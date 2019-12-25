#ifndef LIBDSI_EXCEPTIONS_H
#define LIBDSI_EXCEPTIONS_H

#include <libdsi/arm9/dsi9.h>

//this is only defined for ARM9 because the ARM7 bios is loaded at 0x00000000 and therefore we can't hijack the exception vectors for it

namespace dsi::except {
    /**
     * The default exception base.
     *
     * Also the base address of BIOS memory.
     */
    constexpr u32 DEFAULT_EXCEPTION_BASE = 0xFFFF0000;

    /**
     * The alternate exception base, which may be selected by clearing bit 13 of register CP15,C1,C0,0.
     *
     * Also the base address of ITCM.
     */
    constexpr u32 ALT_EXCEPTION_BASE = 0x00000000;

    /**
     * The different ARM exceptions that can occur.
     */
    enum Exception {
        EXCEPTION_RESET,
        EXCEPTION_UNDEFINED_INSTRUCTION,
        EXCEPTION_SOFTWARE_INTERRUPT,
        EXCEPTION_PREFETCH_ABORT,
        EXCEPTION_DATA_ABORT,
        EXCEPTION_ADDRESS_EXCEEDS_26BIT,
        EXCEPTION_INTERRUPT,
        EXCEPTION_FAST_INTERRUPT
    };

    /**
     * Encodes a simple branch instruction to the given target address.
     *
     * Will crash if the target address is outside of the valid range for a branch placed at the given exception vector.
     *
     * @param exception the exception vector that the branch instruction will be placed at
     * @param target    the address that the branch is targetting
     * @return a branch instruction to be placed at the given exception vector
     */
    u32 encodeBranch(Exception exception, void* target);

    /**
     * Encodes a simple branch instruction to the given target address when placed at the given source address.
     *
     * Will crash if the target address is outside of the valid range for a branch placed at the given source address.
     *
     * @param source the address that the branch instruction will be placed in
     * @param target the address that the branch is targetting
     * @return a branch instruction to be placed at the given source address
     */
    u32 encodeBranch(void* source, void* target);

    /**
     * Makes the given exception be handled by the system BIOS.
     *
     * @param exception the exception to delegate to the system BIOS
     */
    void delegateToBIOS(Exception exception);

    /**
     * "Hijacks" the exception vectors from the BIOS by modifying bit 13 of CP15,C1,C0,0.
     *
     * Called by internal initialization code, shouldn't really be accessed by user code unless you know what you're doing.
     *
     * @param resetToBIOS if false, exception vectors will be set to 0x00000000, otherwise they will be reset to 0xFFFF0000 to allow the BIOS to function normally
     */
    extern "C" void hijackExceptionVectors(bool resetToBIOS);

    /**
     * Gets the current exception vector base.
     */
    extern "C" void* currentVectorBase();
}

#endif //LIBDSI_EXCEPTIONS_H
