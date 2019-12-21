#include <dsi.h>

#if true //libnds hackery (temporary)
#include <nds.h>
#include <stdio.h>

struct __TransferRegion {
    vs16 touchX, touchY;
    vs16 touchXpx, touchYpx;
    vs16 touchZ1, touchZ2;
    vu16 buttons;
    time_t unixTime;
    struct __bootstub* bootcode;
};

#define transfer ((__TransferRegion*)(0x02FFF000))

bool __dsimode; //set in crt0
extern time_t* punixTime;

//void __libnds_exit(int rc);
//int __libnds_gtod(struct _reent* ptr, struct timeval* tp, struct timezone* tz);
#endif

//extern dsi::Void __interruptHandlers[32];

namespace dsi {
    extern "C" void initSystem() {
        //reg::IME = 0;

        for (u32 i = 0; i < 4; i++) { dma::channel(0)->erase(); }

        //clear display registers
        mem::fastClear((void*) 0x04000000, 0x56);
        mem::fastClear((void*) 0x04001000, 0x56); //TODO: figure out why libnds uses 0x04001008 here and then uses videoSetModeSub
        //videoSetModeSub(0);

        video::resetBrightness(video::DISPLAY_BOTH);

        //TODO: replace all of these things

        video::resetVRAM();

        #ifdef ARM9
        //intr::init();

        //libnds stuff that all needs to be replaced
        irqInit();
        fifoInit();
        #else
        //intr::init();

        //libnds stuff that all needs to be replaced
        irqInit();
        fifoInit();
        #endif

        fifoSetValue32Handler(FIFO_SYSTEM, systemValueHandler, nullptr);
        fifoSetDatamsgHandler(FIFO_SYSTEM, systemMsgHandler, nullptr);

        transfer->buttons = 0xFFFF;

        punixTime = &transfer->unixTime;

        //__syscalls.exit = __libnds_exit;
        //__syscalls.gettod_r = __libnds_gtod;

        extern char* fake_heap_end;
        transfer->bootcode = (__bootstub*) fake_heap_end;
        irqEnable(IRQ_VBLANK);
    }

    namespace sys   {
        void powerOn(u32 val)   {
            reg::POWCNT1 |= val;
        }

        void powerOff(u32 val)   {
            reg::POWCNT1 &= ~(val & ~POWER_LCD);
        }
    }
}
