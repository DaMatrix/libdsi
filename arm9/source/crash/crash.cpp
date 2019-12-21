#include <dsi.h>

#if true //libnds hackery (temporary)
#include <nds.h>
#include <stdio.h>
#endif

using namespace dsi;

extern "C" void _crash_doCrash(const char* message, const u32* sp, const u32* stackBase)    {
    u32 lr;
    asm volatile("mov %0, lr" : "=r" (lr));
    auto lr_mode = lr & 1 ? "THUMB" : "ARM";
    lr -= 4 - 3 * (lr & 1);

    dsi::initSystem();

    sys::powerOn(sys::POWER_2D_A | sys::POWER_2D_B);
    /*video::setBackgroundMode(video::DISPLAY_A, video::BG_MODE_0);
    video::setDisplayMode(video::DISPLAY_A, video::DISPLAY_MODE_2D);
    consoleDemoInit();*/

    //TODO: libnds stuff that needs to be eliminated
    auto topConsole = (PrintConsole*) malloc(sizeof(PrintConsole));
    auto bottomConsole = (PrintConsole*) malloc(sizeof(PrintConsole));

    if (true) {
        mem::fastCopy(consoleGetDefault(), topConsole, sizeof(PrintConsole));
        mem::fastCopy(consoleGetDefault(), bottomConsole, sizeof(PrintConsole));
    } else if (false)   {
        memcpy(topConsole, consoleGetDefault(), sizeof(PrintConsole));
        memcpy(bottomConsole, consoleGetDefault(), sizeof(PrintConsole));
    } else if (false)    {
        bios::cpuFastSet_copy(consoleGetDefault(), topConsole, (sizeof(PrintConsole) + 3) >> 2);
        bios::cpuFastSet_copy(consoleGetDefault(), bottomConsole, (sizeof(PrintConsole) + 3) >> 2);
    }

    video::setBackgroundMode(video::DISPLAY_A, video::BG_MODE_0);
    video::setDisplayMode(video::DISPLAY_A, video::DISPLAY_MODE_2D);
    reg::VRAMCNT_A = reg::A_Background0;

    //video::setBackgroundMode(video::DISPLAY_B, video::BG_MODE_0);
    //video::setDisplayMode(video::DISPLAY_B, video::DISPLAY_MODE_2D); //TODO: why do these two not work when they do for engine A?
    videoSetModeSub(MODE_0_2D);
    //vramSetBankC(VRAM_C_SUB_BG);
    reg::VRAMCNT_C = reg::C_BackgroundB;

    consoleInit(topConsole, topConsole->bgLayer, BgType_Text4bpp, BgSize_T_256x256, topConsole->mapBase, topConsole->gfxBase, true, true);
    consoleInit(bottomConsole, bottomConsole->bgLayer, BgType_Text4bpp, BgSize_T_256x256, bottomConsole->mapBase, bottomConsole->gfxBase, false, true);

    video::setBackdrop(video::DISPLAY_BOTH, argb16(1, 25, 0, 0));

    video::topDisplay(video::DISPLAY_A);

    consoleSelect(topConsole);
    iprintf("\x1b[97m\x1b[1;1HError at 0x%08x (%s)\n", lr, lr_mode);
    if (message != nullptr) {
        iprintf("\x1b[3;1H%s\n", message);
    }

    while (true) bios::vBlankIntrWait();
}

#if false
extern "C" void crashSystem(const char* message) {
    u32 lr;
    asm volatile("mov %0, lr" : "=r" (lr));
    auto lr_mode = lr & 1 ? "THUMB" : "ARM";
    lr -= 4 - 3 * (lr & 1);

    initSystem();

    sys::powerOn(sys::POWER_2D_A | sys::POWER_2D_B);
    video::setBackgroundMode(video::DISPLAY_A, video::BG_MODE_0);
    video::setDisplayMode(video::DISPLAY_A, video::DISPLAY_MODE_2D);
    consoleDemoInit();

    video::setBackdrop(video::DISPLAY_BOTH, argb16(1, 25, 0, 0));

    iprintf("\x1b[97m\x1b[1;1HError at 0x%08x (%s)\n", lr, lr_mode);
    if (message != nullptr) {
        iprintf("\x1b[3;1H%s\n", message);
    }

    //iprintf("%08x\n", &__interruptHandlers);

    video::topDisplay(video::DISPLAY_B);

    while (true) bios::vBlankIntrWait();
}
#endif
