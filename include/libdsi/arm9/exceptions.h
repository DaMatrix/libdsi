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
     * "Hijacks" the exception vectors from the BIOS by modifying bit 13 of CP15,C1,C0,0.
     */
    extern "C" void hijackExceptionVectors();
}

#endif //LIBDSI_EXCEPTIONS_H
