#include <dsi.h>

#if true //libnds hackery (temporary)
#include <nds.h>

struct __TransferRegion {
    vs16   touchX, touchY;
    vs16   touchXpx, touchYpx;
    vs16   touchZ1, touchZ2;
    vu16   buttons;
    time_t unixTime;
    struct __bootstub* bootcode;
};

#define transfer ((__TransferRegion*)(0x02FFF000))

extern time_t* punixTime;

//void __libnds_exit(int rc);
//int __libnds_gtod(struct _reent* ptr, struct timeval* tp, struct timezone* tz);

extern "C" void fifoInternalSendInterrupt();
extern "C" void fifoInternalRecvInterrupt();
#endif

//bool __dsimode; //set in crt0

//allocate space in bss for an internal stack used during init and error handling
dsi::u8 __internal_dummy_stack[1024 * 3];

namespace dsi {
    extern "C" void resetSystem() {
        reg::IME = 0;

        for (u32 i = 0; i < 4; i++) { dma::channel(0)->erase(); }

        sys::powerOn(sys::POWER_2D_A | sys::POWER_2D_B | sys::POWER_3D_GEMOETRY | sys::POWER_3D_RENDERING);

        //clear display registers for engine A
        mem::fastClear((void*) 0x04000000, 0x56);

        //clear display registers for engine B
        //this writes to an undefined register at 0x04001008, but that shouldn't cause any problems
        mem::fastClear((void*) 0x04001000, 0x56);

        video::resetBrightness(video::DISPLAY_BOTH);

        sys::powerOff(sys::POWER_3D_GEMOETRY | sys::POWER_3D_RENDERING);

        //video::resetBrightness(video::DISPLAY_BOTH);

        video::resetVRAM();

        intr::init();

        fifoInit();

        intr::set(intr::IPC_SEND_FIFO_EMPTY, fifoInternalSendInterrupt);
        intr::set(intr::IPC_RECV_FIFO_NOT_EMPTY, fifoInternalRecvInterrupt);

        transfer->buttons = 0xFFFF;

        punixTime = &transfer->unixTime;

        //__syscalls.exit = __libnds_exit;
        //__syscalls.gettod_r = __libnds_gtod;

        extern char* fake_heap_end;
        transfer->bootcode = (__bootstub*) fake_heap_end;
        irqEnable(IRQ_VBLANK);
    }

    namespace sys {
        void powerOn(u32 val) {
            reg::POWCNT1 |= val;
        }

        void powerOff(u32 val) {
            reg::POWCNT1 &= ~(val & ~POWER_LCD);
        }
    }
}
