#include "main9.h"

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>

using namespace dsi;

extern "C" void doMain(void* buf);

extern u32 __sp_irq;

void vblankCauseError() {
    DSI_ASSERT(1 + 1 == 3, "Crash from VBlank!")
}

int main() {
#if false
    sys::powerOn(sys::POWER_2D_A | sys::POWER_2D_B);

    auto topConsole    = (PrintConsole*) malloc(sizeof(PrintConsole));
    auto bottomConsole = (PrintConsole*) malloc(sizeof(PrintConsole));

    mem::fastCopy(consoleGetDefault(), topConsole, sizeof(PrintConsole));
    mem::fastCopy(consoleGetDefault(), bottomConsole, sizeof(PrintConsole));

    video::setBackgroundMode(video::DISPLAY_A, video::BG_MODE_0);
    video::setDisplayMode(video::DISPLAY_A, video::DISPLAY_MODE_2D);
    reg::VRAMCNT_A = reg::A_Background0;
#elif false
    crashSystem("Debug crash!");
#elif true
    video::topDisplay(video::DISPLAY_A);

    consoleDemoInit();
    iprintf("%08x %08x\n", mem::fastCopy, mem::fastClear);
    iprintf(isDSiMode() ? "DSi mode!\n" : "NTR mode...\n");

    sys::powerOn(sys::POWER_2D_A | sys::POWER_2D_B);
    iprintf("%08x\n", reg::DISPCNT_A);
    reg::DISPCNT_A = MODE_3_2D;
    iprintf("%08x\n", REG_DISPCNT);

    //auto buf = (u16*) 0x06000000;
    auto buf = bgGetGfxPtr(bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0));
    //auto buf = DISPLAY_TOP;

    video::setBrightness(video::DISPLAY_A, -15);

    doMain(buf);

    if (false)  {
        crashSystem("Debug crash!");
    } else {
        intr::set(intr::VBLANK, vblankCauseError);
        while (true);
    }
#endif
}
