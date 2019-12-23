#include <dsi.h>
#include <libdsi/registers.h>

extern "C" void stdRootHandler();

extern "C" volatile dsi::u32 __irq_flags;
extern "C" volatile dsi::Void __irq_vector;

extern "C" volatile dsi::Void __debug_vector;

namespace dsi::intr {
    namespace _internal {
        void clearHandlers();
    }

    void init() {
        reg::IME = 0;
        reg::IE = 0;
        reg::IF = ~0;

        __debug_vector = nullptr;
        setRootHandler(stdRootHandler);

        _internal::clearHandlers();

        reg::IME = 1;
    }

    void setRootHandler(Void handler)    {
        __irq_vector = handler;
    }
}
