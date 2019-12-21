#include <dsi.h>

#if true //libnds hackery (temporary)
#include <nds.h>
#include <stdio.h>
#endif

using namespace dsi;

struct Snapshot {
    u32 registers[16];
    u32 cpsr;
    u32 spsr;
};

__attribute__((section(".bss"))) Snapshot __crash_snapshot;

extern "C" __attribute__((target("arm"))) void _crash_doCrash(const char* message) {
    dsi::initSystem();

    sys::powerOn(sys::POWER_2D_A | sys::POWER_2D_B);

    auto topConsole    = (PrintConsole*) malloc(sizeof(PrintConsole));
    auto bottomConsole = (PrintConsole*) malloc(sizeof(PrintConsole));

    mem::fastCopy(consoleGetDefault(), topConsole, sizeof(PrintConsole));
    mem::fastCopy(consoleGetDefault(), bottomConsole, sizeof(PrintConsole));

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
    iprintf("\x1b[97m\n");
    {
        u32 lr = __crash_snapshot.registers[14];
        iprintf(" Error at 0x%08x (%s)\n", lr - 4 + 3 * (lr & 1), lr & 1 ? "THUMB" : "ARM");
    }

    if (message != nullptr) {
        iprintf("\n %s\n", message);
    }

    consoleSelect(bottomConsole);
    iprintf("\x1b[97m\n");

    iprintf(" CPSR 0x%08x\n", __crash_snapshot.cpsr);
    iprintf(" SPSR 0x%08x\n", __crash_snapshot.spsr);

    for (u32 i = 0; i <= 12; i++)    {
        iprintf(" r%-3d 0x%08x\n", i, __crash_snapshot.registers[i]);
    }

    iprintf(" SP   0x%08x\n", __crash_snapshot.registers[13]);
    iprintf(" LR   0x%08x\n", __crash_snapshot.registers[14]);
    iprintf(" PC   0x%08x\n", __crash_snapshot.registers[15]);

    while (true) { bios::vBlankIntrWait(); }
}
