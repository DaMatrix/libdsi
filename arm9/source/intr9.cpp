#include <dsi.h>

//place interrupt handlers in itcm on ARM9
__attribute__((section(".itcm.__interruptHandlers"))) dsi::Void __interruptHandlers[32];

namespace dsi::intr {
    namespace _internal {
        void clearHandlers() {
            mem::fastClear(__interruptHandlers, sizeof(__interruptHandlers));
        }
    }

    void set(Interrupt interrupt, Void handler) {
        DSI_ASSERT(false, "Message!")
    }
}
