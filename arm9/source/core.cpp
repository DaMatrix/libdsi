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

namespace dsi {
    extern "C" void initSystem() {
        for (u32 i = 0; i < 4; i++) { dma::channel(0)->erase(); }

        memory::fastClear((void*) 0x04000000, 0x56);
        memory::fastClear((void*) 0x04001008, 0x56);

        //TODO: replace all of these things
        videoSetModeSub(0);

        video::resetVRAM();

        //libnds stuff that all needs to be replaced
        irqInit();
        fifoInit();

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

    extern "C" void crashSystem(const char* message) {
        register u32 _lr asm("lr");
        u32 lr = _lr - (lr & 1 ? 2 : 4);

        initSystem();

        //TODO: replace all of these things
        powerOn(POWER_ALL_2D);
        videoSetModeSub(MODE_3_2D);

        setBrightness(1, -16);

        consoleDemoInit();

        iprintf("\x1b[41m\x1b[1;1HInternal error at 0x%08x\n", lr);
        if (message != nullptr) {
            iprintf("\x1b[2;1H%s\n", message);
        }

        while (true) { bios::vBlankIntrWait(); }
    }
}
