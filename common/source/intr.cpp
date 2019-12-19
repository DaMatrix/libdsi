#include <dsi.h>

extern "C" void stdRootHandler();

extern "C" volatile dsi::u32& __irq_flags;
extern "C" volatile dsi::Void& __irq_vector;

#ifdef ARM9
//place interrupt handlers in dtcm on ARM9
__attribute__((section(".dtcm")))
#endif
void* __interruptHandlers[32];

#ifdef ARM7
//additional interrupt handlers for ARM7
void* __interruptHandlers7[32];
#endif

namespace dsi::intr {
    void init() {
        //clear handlers
        mem::fastClear(__interruptHandlers, sizeof(__interruptHandlers));
#ifdef ARM7
        mem::fastClear(__interruptHandlers7, sizeof(__interruptHandlers7));
#endif

        setRootHandler(stdRootHandler);
    }

    void setRootHandler(Void handler)    {
        __irq_vector = handler;
    }
}
