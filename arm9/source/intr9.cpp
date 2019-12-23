#include <dsi.h>

//place interrupt handlers in dtcm on ARM9
__attribute__((section(".dtcm.__interruptHandlers"))) dsi::Void __interruptHandlers[32];

namespace dsi::intr {
    namespace _internal {
        void clearHandlers() {
            mem::fastClear(__interruptHandlers, sizeof(__interruptHandlers));
        }
    }

    void set(Interrupt interrupt, Void handler) {
        DSI_ASSERT((interrupt & 0x1F) == interrupt, "Invalid interrupt!")

        __interruptHandlers[interrupt] = handler;
    }
}
