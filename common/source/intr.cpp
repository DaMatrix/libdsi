#include <dsi.h>
#include <libdsi/registers.h>

extern "C" void stdRootHandler();
extern "C" void stdRootHandler_test();

//extern "C" volatile dsi::u32 __irq_flags;
extern "C" volatile dsi::Void __irq_vector;

#ifdef ARM9
//place interrupt handlers in dtcm on ARM9
//__attribute__((section(".dtcm.__interruptHandlers")))
#endif
dsi::Void __interruptHandlers[32];

/*
#ifdef ARM7
//additional interrupt handlers for ARM7
dsi::Void* __interruptHandlers7[32];
#endif
*/

extern "C" void IntrMain();

extern "C" void fifoInternalSendInterrupt();
extern "C" void fifoInternalRecvInterrupt();

extern "C" void test_init_irq();

namespace dsi::intr {
    void init() {
        reg::IME = 0;
        reg::IE = 0;
        reg::IF = ~0;

        //setRootHandler(stdRootHandler);
        //setRootHandler(stdRootHandler_test);
        setRootHandler(IntrMain);

        test_init_irq();

        //clear handlers
        /*for (u32 i = 0; i < 32; i++)    {
            __interruptHandlers[i] = nullptr;
        }*/
        //mem::fastClear(__interruptHandlers, sizeof(__interruptHandlers));
#ifdef ARM7
        //mem::fastClear(__interruptHandlers7, sizeof(__interruptHandlers7));
#endif

        //reg::IME = 1;
    }


    void setRootHandler(Void handler)    {
        __irq_vector = handler;
    }

    void set(u32 interrupt, Void handler){
        //__interruptHandlers[interrupt] = handler;
    }
}
