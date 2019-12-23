#ifndef LIBDSI_CORE_H
#define LIBDSI_CORE_H

#include <libdsi/types.h>
#include <libdsi/version.h>

//internal macros
#define __DSI_STRINGIZE(expr) __DSI_DO_STRINGIZE(expr)
#define __DSI_DO_STRINGIZE(expr) #expr

#define __DSI_CURRENT_LINE __DSI_STRINGIZE(__LINE__)
#define __DSI_CURRENT_FILE __FILE__
//#define __DSI_CURRENT_FUNC __PRETTY_FUNCTION__ //TODO: make this work? (apparently this isn't a preprocessor macro for some reason)

#define __DSI_OVERLOAD_MACRO_2(ARG1, ARG2, NAME, ...) NAME

//#define __DSI_ASSERT_MSG(EXPR, MSG) if (!(EXPR)) dsi::crashSystem("At " __DSI_CURRENT_FILE ":" __DSI_CURRENT_LINE " (" __DSI_CURRENT_FUNC "): " MSG);
//#define __DSI_ASSERT_NOMSG(EXPR) if (!(EXPR)) dsi::crashSystem("Assertion failed at " __DSI_CURRENT_FILE ":" __DSI_CURRENT_LINE " (" __DSI_CURRENT_FUNC ")");
#define __DSI_ASSERT_MSG(EXPR, MSG) if (!(EXPR)) dsi::crashSystem("Assertion failed at " __DSI_CURRENT_FILE ":" __DSI_CURRENT_LINE ": " MSG);
#define __DSI_ASSERT_NOMSG(EXPR) if (!(EXPR)) dsi::crashSystem("Assertion failed at " __DSI_CURRENT_FILE ":" __DSI_CURRENT_LINE);

//actual macros
#define DSI_ASSERT(...) __DSI_OVERLOAD_MACRO_2(__VA_ARGS__, __DSI_ASSERT_MSG, __DSI_ASSERT_NOMSG)(__VA_ARGS__)

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

    namespace sys   {
        /**
         * Initializes the system.
         *
         * Should probably never be called by user code.
         */
        extern "C" void resetSystem();

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

        /**
         * @return the current value of the SP register
         */
        extern "C" u32 getSP();

        /**
         * @return the current value of the CPSR register
         */
        extern "C" u32 getCPSR();

        /**
         * @return the current value of the SPSR register
         */
        extern "C" u32 getSPSR();

        /**
         * Checks if the system is currently running in DSi mode.
         *
         * By the time user code starts running, it should be impossible for this to return anything other than true.
         *
         * This will also update the condition flags, allowing use of "eq" if DSi mode is active and "ne" if it is not.
         *
         * @return whether or not the system is currently running in DSi mode
         */
        extern "C" bool checkDSiMode();

        /**
         * Switches to the internal stack, a 3x3KiB region in the bss section used during setup.
         */
        extern "C" void switchToInternalStack();

        /**
         * Switches to the DTCM stack, which is the actual stack used by user code.
         *
         * Generally this should be the only stack that is used, due to DTCM being significantly faster than other memory regions.
         */
        extern "C" void switchToDTCMStack();

        /**
         * Switches to the debug stack, a small region hard-coded into the BIOS intended to be used by the debug interrupt handler.
         *
         * The stack base is different for ARM7 and ARM9:
         * ARM9: 0x02FFFD9C
         * ARM7: 0x0380FFDC
         */
        extern "C" void switchToDebugStack();

        /**
         * The different ARM CPU modes.
         */
        enum CPUMode {
            MODE_USER = 0x10,
            MODE_FIQ = 0x11,
            MODE_FAST_INTERRUPT = MODE_FIQ,
            MODE_IRQ = 0x12,
            MODE_INTERRUPT = MODE_IRQ,
            MODE_SWI = 0x13,
            MODE_SUPVERVISOR = MODE_SWI,
            MODE_ABORT = 0x17,
            MODE_UNDEFINED = 0x1B,
            MODE_SYSTEM = 0x1F
        };

        /**
         * @return the current ARM CPU mode
         */
        extern "C" CPUMode getCurrentMode();

        /**
         * Switches to the given ARM CPU mode.
         *
         * This is highly likely to cause issues of some kind due to e.g. the stack pointer being replaced. Should probably not be called from anything
         * except assembly...
         *
         * @param mode the new mode to switch to
         */
        extern "C" void switchToMode(CPUMode mode);
    }
}

#endif //LIBDSI_CORE_H
