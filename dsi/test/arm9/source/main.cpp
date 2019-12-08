#include "main.h"

void vblank() {
    Display::CURRENT_FRAME++;

    scanKeys();
    Display::KEYS_DOWN |= keysDown();
    touchRead(&Display::TOUCH_POS);

    /*for (std::vector<InputHandler>::iterator it = Gui::HANDLERS.begin(); it != Gui::HANDLERS.end(); it++) {
        (*it)(Gui::KEYS_DOWN, &Gui::TOUCH_POS);
    }*/

    if (Display::QUEUED_REDRAW || !(Display::CURRENT_FRAME & 0xF)) {
        memcpy(Display::DISPLAY_TOP, Display::TEMP_DISPLAY_TOP, 256 * 256 * sizeof(u16));
        memcpy(Display::DISPLAY_BOTTOM, Display::TEMP_DISPLAY_BOTTOM, 256 * 256 * sizeof(u16));

        //GuiExitCode exitCode = Gui::MENU_STACK.back()(keys);

        Display::QUEUED_REDRAW = false;
    }
}

typedef struct {
    int w;
    int h;
    u16 pixels[32 * 32];
} Icon;

inline int endlessWait() {
    while (true) {
        dsi::bios::vBlankIntrWait();
    }
    return 0;
}

void swapByteOrder(u32& ui) { ui = (ui >> 24) | ((ui << 8) & 0x00FF0000) | ((ui >> 8) & 0x0000FF00) | (ui << 24); }

int main() {
    Font::init();
    irqSet(IRQ_VBLANK, vblank);

    if (true)   {
        u32 size = 64 << 3;
        auto a = new u32[size];
        auto b = new u32[size];
        for (u32 i = 0; i < size; i++)  {
            a[i] = i + 1;
        }
        Display::TOP->printf("%d %d %d %d %d", a[0], a[1], a[2], a[3], a[4]);
        Display::TOP->printf("%d %d %d %d %d", b[0], b[1], b[2], b[3], b[4]);
        dsi::memory::copyChunks32(a, b, size >> 3);
        Display::TOP->printf("%d %d %d %d %d", a[0], a[1], a[2], a[3], a[4]);
        Display::TOP->printf("%d %d %d %d %d", b[0], b[1], b[2], b[3], b[4]);
        for (u32 i = 0; i < size; i++)  {
            if (a[i] != b[i])   {
                Display::TOP->printf("%d: %d != %d", i, a[i], b[i]);
                goto COPY_TEST_END;
            }
        }
        Display::TOP->printf("Copy test succeeded!");
    }
    COPY_TEST_END:

    if (true) {
        for (u32 i = 0; false && i <= 10; i++) {
            Display::TOP->printf("%d, %d, %d, %d, %d", dsi::bios::div(i, 1), dsi::bios::div(i, 2), dsi::bios::div(i, 3), dsi::bios::div(i, 4), dsi::bios::div(i, 5));
        }
        Display::TOP->print("Done!");
        //Display::TOP->print("Done! 2");

        for (int i = 0; i < 120; i++) dsi::bios::vBlankIntrWait();

        {
            u32 digest[5];

            dsi::bios::sha1_init_update_finish(digest, nullptr, 0);
            for (size_t i = 5; i--;) { swapByteOrder(digest[i]); }
            Display::TOP->printf("%08x%08x%08x%08x%08x", digest[0], digest[1], digest[2], digest[3], digest[4]);

            const char* text = "The quick brown fox jumps over the lazy dog";
            dsi::bios::sha1_init_update_finish(digest, text, strlen(text));
            for (size_t i = 5; i--;) { swapByteOrder(digest[i]); }
            Display::TOP->printf("%08x%08x%08x%08x%08x", digest[0], digest[1], digest[2], digest[3], digest[4]);
        }

        //Display::TOP->printf("0x%08x", dsi::reg::KEYINPUT());
        //for (int i = 0; i < 120; i++) swiWaitForVBlank();
        //Display::TOP->printf("0x%08x", dsi::reg::KEYINPUT());

        /*for (int i = 1; true; i++)  {
            for (int i = 0; i < 30; i++) swiWaitForVBlank();
            new char[524288 << 1];
            Display::TOP->printf("Allocated %d MB", i);
        }*/ //there are >15mb ram, as there should be

        /*int* randomHeapPtr = new int;
        Display::TOP->printf("Normal: 0x%08x", (int) randomHeapPtr);
        Display::TOP->printf("Cached: 0x%08x", (int) memCached(randomHeapPtr));
        Display::TOP->printf("Uncached: 0x%08x", (int) memUncached(randomHeapPtr));*/

        /*int iterations = 0;
        Display::TOP->print("Waiting...");
        while (iterations++ != 120) swiWaitForVBlank();

        Display::TOP->print("Checking data...");
        auto ptr = (u32*) 0x03000000;
        int i = 0;
        while (ptr[i] == (0xBEEF0000 | i) && i != 8192) i++;
        if (i == 8192)  {
            Display::TOP->print("Data valid!");
        } else {
            Display::TOP->printf("Invalid! %d %d", i << 2, i);
        }*/

        /*fifoInit();
        fifoSetValue32Handler(FIFO_USER_01, [](u32 value32, void* userdata) {
            Display::TOP->printf("received data: %d", value32);
            switch(value32 & 0xF) {
                case 0:
                    Display::TOP->print("ARM7 is writing data...");
                    break;
                case 1:
                    Display::TOP->print("Checking data...");
                    auto ptr = (u32*) (value32 >> 4);
                    int i = 0;
                    while (ptr[i++] != (0xBEEF0000 | i) && i != 8192);
                    if (i == 8192)  {
                        Display::TOP->print("Data valid!");
                    } else {
                        Display::TOP->printf("Invalid! %d %d", i << 2, i);
                    }
                    break;
            }
        }, nullptr);

        Display::TOP->print("Waiting...");*/

        return endlessWait();
    }

    if (!isDSiMode()) {
        drawText(5, 5, ARGB16(1, 31, 0, 0), BOTTOM, "Must be used on DSi!");
        return endlessWait();
    } else {
        setCpuClock(true);

        Display::TOP->printf("Battery level: %d, heap is %d bytes", (int) getBatteryLevel(), (int) (getHeapEnd() - getHeapStart()));
        Display::TOP->printf("(~%d MB) (from %d to %d,", (int) (getHeapEnd() - getHeapStart()) / (1024 * 1024), (int) getHeapStart());
        Display::TOP->printf(" limit: %d)", (int) getHeapLimit());
    }

    Display::TOP->printf("NDS: %d, DSi: %d", sizeof(dsi::wifi::WifiConnectionData), sizeof(dsi::wifi::DSiWifiConnectionData));

    char* personal_data_ptr = (char*) PersonalData;

    {
        auto name = new char[PersonalData->nameLen + 1];
        name[PersonalData->nameLen] = 0;
        for (size_t i = PersonalData->nameLen; i--;) {
            name[i] = (u8) PersonalData->name[i];
        }
        Display::TOP->printf("Name: %s (%d chars)", name, (int) PersonalData->nameLen);
        delete name;
    }

    dsi::wifi::DSiWifiConnectionData* wifi_ptr = (dsi::wifi::DSiWifiConnectionData*) (personal_data_ptr - 0xA00);

    Display::TOP->printf("SSID: %s", wifi_ptr[1].ssid);

    if (true) { return endlessWait(); }

    /*Display::HANDLERS.push_back([](int keys, touchPosition *touch) -> void {
        if (keys & KEY_START) {
            Display::TOP->print("Closing socket...");
            Socket::INSTANCE.close();
            //exit(0);
        }
    });*/

    //Display::TOP->print("Waiting...");
    //while (Display::CURRENT_FRAME < 200) swiWaitForVBlank();

    try {
        Display::TOP->print("Connecting to server...");
        //Socket::INSTANCE.open("192.168.1.108", 8234);
        Display::TOP->print("Connected!");

        //Display::MENU_STACK.push_back(gui_loading);

        //int counter = 0;
        while (true) {
            swiWaitForVBlank();
            if (Display::KEYS_DOWN & KEY_A) {
                /*Message *response = Socket::INSTANCE.sendAndWaitForResponse(new Message(2, "Hello server!"));
                Display::BOTTOM->printf("Server response: %s", response->data);
                delete response;*/
            } else if (Display::KEYS_DOWN & KEY_B) {
                /*Message *response = Socket::INSTANCE.sendAndWaitForResponse(new Message(3, (char*) &counter, 4, true));
                counter++;
                Icon* icon = (Icon*) response->data;
                Display::TOP->printf("Loading %dx%d bitmap...", icon->w, icon->h);
                for (int x = icon->w - 1; x >= 0; x--)    {
                    for (int y = icon->h - 1; y >= 0; y--)    {
                        Display::TEMP_DISPLAY_BOTTOM[x | (y << 8)] = icon->pixels[(icon->w - 1 - x) * icon->w + icon->h - 1 - y];
                    }
                }
                Display::TOP->print("Done!");
                delete response;
                Display::QUEUED_REDRAW = true;*/
            }

            Display::KEYS_DOWN = 0;
            if (min(1, 2) == 3) {
                //this is just to trick my IDE into thinking that this loop isn't infinite
                break;
            }
        }
    } catch (const char* e) {
        drawText(5, 5, ARGB16(1, 31, 0, 0), BOTTOM, e);
    } catch (void* e) {
        drawText(5, 5, ARGB16(1, 31, 0, 0), BOTTOM, "Exception");
    }
    //Socket::INSTANCE.close();
    return endlessWait();
}

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int clamp(int a, int min, int max) {
    return a < min ? min : a > max ? max : a;
}

const char* fmt(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = new char[vsnprintf(nullptr, 0, format, args) + 1];
    vsprintf(result, format, args);
    return result;
}
