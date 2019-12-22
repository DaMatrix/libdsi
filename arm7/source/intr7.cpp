#include <dsi.h>

__attribute__((section("twl_bss"))) dsi::Void __interruptHandlers[32];
__attribute__((section("twl_bss"))) dsi::Void __interruptHandlers7[32];

namespace dsi::intr {
    namespace _internal {
        void clearHandlers() {
            mem::fastClear(__interruptHandlers, sizeof(__interruptHandlers));
            mem::fastClear(__interruptHandlers7, sizeof(__interruptHandlers7));
        }
    }

    void set(Interrupt interrupt, Void handler) {
        //TODO: DSI_ASSERT((interrupt & 0x1F) == interrupt, "Invalid interrupt!")

        if (interrupt & 0x20) {
            __interruptHandlers7[interrupt] = handler;
        } else {
            __interruptHandlers[interrupt] = handler;
        }
    }
}
