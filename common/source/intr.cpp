#include <dsi.h>
#include <libdsi/registers.h>

extern "C" void stdRootHandler();
extern "C" void stdRootHandler_test();

//extern "C" volatile dsi::u32 __irq_flags;
extern "C" volatile dsi::Void __irq_vector;

extern "C" void IntrMain();

extern "C" void test_init_irq();

namespace dsi::intr {
    namespace _internal {
        void clearHandlers();
    }

    void init() {
        reg::IME = 0;
        reg::IE = 0;
        reg::IF = ~0;

        setRootHandler(stdRootHandler);
        //setRootHandler(stdRootHandler_test);
        //setRootHandler(IntrMain);

        test_init_irq();

        _internal::clearHandlers();

        reg::IME = 1;
    }

    void setRootHandler(Void handler)    {
        __irq_vector = handler;
    }
}
