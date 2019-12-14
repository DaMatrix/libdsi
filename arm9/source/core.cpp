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

        //clear display registers
        mem::fastClear((void*) 0x04000000, 0x56);
        mem::fastClear((void*) 0x04001000, 0x56); //TODO: figure out why libnds uses 0x04001008 here

        //TODO: replace all of these things
        videoSetModeSub(0);

        video::resetVRAM();

        if (isDSiMode()) setCpuClock(true);

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
        u32 lr = _lr - 2 * (lr & 1);
        const char* lr_mode = lr & 1 ? "THUMB" : "ARM";

        initSystem();

        //TODO: replace all of these things
        powerOn(POWER_ALL_2D);
        lcdSwap();
        videoSetMode(MODE_3_2D);
        mem::fastFillHalfWords(ARGB16(0, 0, 0, 0), bgGetGfxPtr(bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0)), SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(u16));

        consoleDemoInit();

        iprintf("\x1b[41m\x1b[1;1HError at 0x%08x (%s)\n", lr, lr_mode);
        if (message != nullptr) {
            iprintf("\x1b[3;1H%s\n", message);
        }

        while (true) { bios::vBlankIntrWait(); }
    }
}
