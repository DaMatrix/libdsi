#include <dsi.h>

extern "C" void stdRootHandler();

extern "C" volatile dsi::u32& __irq_flags;
extern "C" volatile dsi::Void& __irq_vector;

namespace dsi::intr {
    void init() {
        setRootHandler(stdRootHandler);
    }

    void setRootHandler(Void handler)    {
        __irq_vector = handler;
    }
}
