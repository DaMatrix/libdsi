#ifndef LIBDSI_CORE_H
#define LIBDSI_CORE_H

#include <libdsi/types.h>
#include <libdsi/version.h>

namespace dsi {
    namespace mem {
        /**
         * Copies the specified memory region to the given destination.
         *
         * Source and destination addresses MUST be word-aligned, otherwise behavior is undefined!
         *
         * @param src  the base address of the source data
         * @param dst  the base address of the destination data
         * @param size the number of bytes to copy. If not word-aligned, will be rounded up!
         */
        extern "C" void fastCopy(const void* src, void* dst, u32 size);

        /**
         * Clears (zeroes out) the specified memory region.
         *
         * Destination address MUST be word-aligned, otherwise behavior is undefined!
         *
         * @param dst  the base address to start clearing from
         * @param size the number of bytes to clear. If not word-aligned, will be rounded up!
         */
        extern "C" void fastClear(void* dst, u32 size);

        /**
         * Fills the specified memory region with the given word value.
         *
         * Destination address MUST be word-aligned, otherwise behavior is undefined!
         *
         * @param value the value to fill the memory region with
         * @param dst   the base address to start filling from
         * @param size  the number of bytes to fill. If not word-aligned, will be rounded up!
         */
        extern "C" void fastFill(u32 value, void* dst, u32 size);

        /**
         * Fills the specified memory region with the given halfword value.
         *
         * Destination address MUST be word-aligned, otherwise behavior is undefined!
         *
         * @param value the value to fill the memory region with
         * @param dst   the base address to start filling from
         * @param size  the number of bytes to fill. If not word-aligned, will be rounded up!
         */
        extern "C" void fastFillHalfWords(u16 value, void* dst, u32 size);
    }

    #ifdef ARM9
    /**
     * Initializes the system.
     *
     * Should probably never be called by user code.
     */
    extern "C" void initSystem();

    extern "C" void _do_initSystem(bool safe);

    /**
     * Crashes the system, resetting most things and displaying a full-screen error message on both displays.
     *
     * This method will never return.
     *
     * Identical to crashSystem(const char*), but never displays a message.
     */
    extern "C" void crashSystemNoMsg();

    /**
     * Crashes the system, resetting most things and displaying a full-screen error message on both displays.
     *
     * This method will never return.
     *
     * @param message a message to display to the user. If nullptr, a message will not be displayed.
     */
    extern "C" void crashSystem(const char* message);
    #endif //ARM9

    namespace sys   {
        /**
         * The different power modes, used for powerOn(u32) and powerOff(u32).
         */
        enum PowerModes: u32 {
            POWER_LCD = 1, //always enabled, disabling can apparently cause hardware damage
            POWER_2D_A = 1 << 1, //2D engine A
            POWER_2D_B = 1 << 9, //2D engine B
            POWER_3D_RENDERING = 1 << 2, //3D rendering engine
            POWER_3D_GEMOETRY = 1 << 3, //3D geometry engine
            POWER_SWAP_DISPLAYS = 1 << 15 //if enabled, engine A output is sent to upper screen
        };

        /**
         * Powers on the given components.
         *
         * @param val the components to enable, see PowerModes
         */
        void powerOn(u32 val);

        /**
         * Powers off the given components.
         *
         * @param val the components to disable, see PowerModes
         */
        void powerOff(u32 val);
    }
}

#endif //LIBDSI_CORE_H
