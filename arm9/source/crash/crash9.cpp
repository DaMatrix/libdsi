#include <dsi.h>

#if true //libnds hackery (temporary)
#include <nds.h>
#include <stdio.h>
#endif

#if false //debug stuff
#define _CRASH_DEBUG
#endif

using namespace dsi;

struct Snapshot {
    u32 registers[16];
    u32 cpsr;
    u32 spsr;
    u32 stack[0x4000 >> 2];
};

__attribute__((section(".bss"))) Snapshot __crash_snapshot;
__attribute__((section(".bss"))) bool __crash_isCrashing;

enum DisplayMode: i32 {
    MODE_REGISTERS,
    MODE_PS,
#ifdef _CRASH_DEBUG
    MODE_CURRENT_PS,
#endif
    MODE_STACK,
    NUM_MODES
};

void _crash_displayPSR(const char* name, u32 val);

extern "C" void _crash_doCrash(const char* message, u32 sp) {
    __crash_isCrashing = true;
    sys::resetSystem();

    PrintConsole topConsole, bottomConsole;

    mem::fastCopy(consoleGetDefault(), &topConsole, sizeof(PrintConsole));
    mem::fastCopy(consoleGetDefault(), &bottomConsole, sizeof(PrintConsole));

    video::setBackgroundMode(video::DISPLAY_A, video::BG_MODE_0);
    video::setDisplayMode(video::DISPLAY_A, video::DISPLAY_MODE_2D);
    reg::VRAMCNT_A = reg::A_Background0;

    //video::setBackgroundMode(video::DISPLAY_B, video::BG_MODE_0);
    //video::setDisplayMode(video::DISPLAY_B, video::DISPLAY_MODE_2D); //TODO: why do these two not work when they do for engine A?
    videoSetModeSub(MODE_0_2D);
    //vramSetBankC(VRAM_C_SUB_BG);
    reg::VRAMCNT_C = reg::C_BackgroundB;

    consoleInit(&topConsole, topConsole.bgLayer, BgType_Text4bpp, BgSize_T_256x256, topConsole.mapBase, topConsole.gfxBase, true, true);
    consoleInit(&bottomConsole, bottomConsole.bgLayer, BgType_Text4bpp, BgSize_T_256x256, bottomConsole.mapBase, bottomConsole.gfxBase, false, true);

    video::setBackdrop(video::DISPLAY_BOTH, argb16(1, 25, 0, 0));

    video::topDisplay(video::DISPLAY_A);

    consoleSelect(&topConsole);
    iprintf("\x1b[97m\n");
    {
        u32 lr = __crash_snapshot.registers[14];
        iprintf(" Error at 0x%08x (%s)\n", lr - 4 + 3 * (__crash_snapshot.cpsr & bit(5)), __crash_snapshot.cpsr & bit(5) ? "THUMB" : "ARM");
    }

    if (message != nullptr) {
        iprintf("\n %s\n", message);
    }

    //iprintf("DSi mode: %u\nSCFG_A9ROM: 0x%08x\n", sys::checkDSiMode(), *((u32*) 0x04004000));

    iprintf(
        "\x1b[15;1HSnapshot Controls:\n"
        "  A: Page forward\n"
        "  B: Page back\n"
        " Stack controls:\n"
        "  Up: Scroll up\n"
        "  Down: Scroll down\n"
    );

    bool needsRender = true;
    i32 currentMode  = 0;

    u32 held    = 0;
    u32 pressed = 0;
    u32 repeat  = 0;

    constexpr u32 NUM_STACK_ROWS = 22;
    constexpr u32 STACK_SIZE = 0x4000 / 4;
    constexpr u32 STACK_MASK = STACK_SIZE - 1;
    u32 stackIndex = STACK_SIZE - NUM_STACK_ROWS;
    u32 scrollingStack = 0;

    while (true) {
        scanKeys();
        held = keysHeld();
        pressed = keysDown();
        repeat = keysDownRepeat();

        if (pressed & KEY_A) {
            if (++currentMode >= NUM_MODES)  {
                currentMode = 0;
            }
            needsRender = true;
        } else if (pressed & KEY_B) {
            if (--currentMode < 0)  {
                currentMode = NUM_MODES - 1;
            }
            needsRender = true;
        } else if (currentMode == MODE_STACK)   {
            if (held & KEY_UP)   {
                if (pressed & KEY_UP || scrollingStack == KEY_UP) {
                    if (++stackIndex >= STACK_SIZE) {
                        stackIndex = 0;
                    }
                    needsRender = true;
                }
                if (!(pressed & KEY_UP) && repeat & KEY_UP) {
                    scrollingStack = KEY_UP;
                }
            } else if (held & KEY_DOWN)   {
                if (pressed & KEY_DOWN || scrollingStack == KEY_DOWN) {
                    if (--stackIndex < 0) {
                        stackIndex = STACK_SIZE - 1;
                    }
                    needsRender = true;
                }
                if (!(pressed & KEY_DOWN) && repeat & KEY_DOWN) {
                    scrollingStack = KEY_DOWN;
                }
            } else {
                scrollingStack = 0;
            }
        }

        if (needsRender) {
            //we need to update the screen
            consoleSelect(&bottomConsole);
            consoleClear();
            iprintf("\x1b[97m\n");

            const char* title = "                ";

            switch (currentMode) {
                case MODE_REGISTERS: {
                    title = "Registers       ";

                    for (u32 i = 0; i <= 12; i++) {
                        iprintf(" r%-3d 0x%08x\n", i, __crash_snapshot.registers[i]);
                    }

                    iprintf(" SP   0x%08x\n", __crash_snapshot.registers[13]);
                    iprintf(" LR   0x%08x\n", __crash_snapshot.registers[14]);
                    iprintf(" PC   0x%08x\n\n", __crash_snapshot.registers[15]);

                    iprintf(" CPSR 0x%08x\n", __crash_snapshot.cpsr);
                    iprintf(" SPSR 0x%08x\n", __crash_snapshot.spsr);

                    iprintf("\n Current SP 0x%08x\n", sp);
                    //iprintf("\n Current SP 0x%08x\n Should be  0x%08x\n", sp, intended_sp);
                    break;
                }
#ifdef _CRASH_DEBUG
                case MODE_CURRENT_PS:
#endif
                case MODE_PS: {
                    title = "PS Registers    ";
                    u32 vals[2];
                    vals[0] = __crash_snapshot.cpsr;
                    vals[1] = __crash_snapshot.spsr;
#ifdef _CRASH_DEBUG
                    if (currentMode == MODE_CURRENT_PS) {
                        title = "Current PS      ";
                        vals[0] = sys::getCPSR();
                        vals[1] = sys::getSPSR();
                    }
#endif
                    _crash_displayPSR("CPSR", vals[0]);
                    _crash_displayPSR("SPSR", vals[1]);
                    break;
                }
                case MODE_STACK: {
                    title = "DTCM (Stack)    ";
                    for (u32 i = 0; i < NUM_STACK_ROWS; i++)    {
                        u32 off = (stackIndex + (NUM_STACK_ROWS - i - 1)) & STACK_MASK;
                        iprintf(" 0x%08x: 0x%08x\n", dtcmStart() + (off << 2), __crash_snapshot.stack[off]);
                    }
                    break;
                }
                default:
                    iprintf("Unknown mode: %d", currentMode);
            }

            consoleSelect(&topConsole);
            iprintf("\x1b[22;1H%d/%d: %s", currentMode + 1, NUM_MODES, title);

            needsRender = false;
        }

        bios::vBlankIntrWait();
    }
}

void _crash_displayPSR(const char* name, u32 val)   {
    iprintf(" %s=0x%08x\n\n", name, val);
    iprintf(" N=%u Z=%u C=%u V=%u Q=%u I=%u F=%u\n", (val >> 31) & 1, (val >> 30) & 1, (val >> 29) & 1, (val >> 28) & 1, (val >> 27) & 1, (val >> 7) & 1, (val >> 6) & 1);
    const char* mode = "Invalid?!?";
    switch (val & mask(5)) {
        case 0x10:
            mode = "User";
            break;
        case 0x11:
            mode = "Fast Interrupt";
            break;
        case 0x12:
            mode = "Interrupt";
            break;
        case 0x13:
            mode = "Supervisor";
            break;
        case 0x16:
            mode = "Monitor";
            break;
        case 0x17:
            mode = "Abort";
            break;
        case 0x1A:
            mode = "Hypervisor";
            break;
        case 0x1B:
            mode = "Undefined";
            break;
        case 0x1F:
            mode = "System";
            break;
    }
    iprintf(" Mode=0x%02x (%s)\n", val & mask(5), mode);
    iprintf(" Execution State=%s\n", val & bit(5) ? "THUMB" : "ARM");
    iprintf(" Endianness=%s\n\n\n", val & bit(9) ? "Big" : "Little");
}
