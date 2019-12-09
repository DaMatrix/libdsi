#include "main.h"

void vblank() {
    Display::CURRENT_FRAME++;

    scanKeys();
    Display::KEYS_DOWN |= keysDown();
    touchRead(&Display::TOUCH_POS);

    if (Display::QUEUED_REDRAW || !(Display::CURRENT_FRAME & 0xF)) {
        dsi::memory::copyChunks32(Display::TEMP_DISPLAY_TOP, Display::DISPLAY_TOP, (SCREEN_WIDTH * SCREEN_HEIGHT * 2) >> 5);
        dsi::memory::copyChunks32(Display::TEMP_DISPLAY_BOTTOM, Display::DISPLAY_BOTTOM, (SCREEN_WIDTH * SCREEN_HEIGHT * 2) >> 5);

        Display::QUEUED_REDRAW = false;
    }
}

int endlessWait() {
    while (true) {
        dsi::bios::vBlankIntrWait();
    }
    return 0;
}

void swapByteOrder(u32& ui) { ui = (ui >> 24) | ((ui << 8) & 0x00FF0000) | ((ui >> 8) & 0x0000FF00) | (ui << 24); }

__attribute__((section(".itcm"))) int main() {
    Font::init();
    //irqSet(IRQ_VBLANK, vblank);

    constexpr int size = 256;
    u32 x = 1;
    u32 buf[size];
    while (true)    {
        for (int i = (SCREEN_WIDTH * SCREEN_HEIGHT / 2 / size) - 1; i >= 0; i--)   {
            for (int j = 0; j < size; j++)    {
                x ^= x << 13;
                x ^= x >> 17;
                x ^= x << 5;
                buf[j] = x | 0x80008000;
            }
            dsi::memory::copyChunks32(&buf, Display::DISPLAY_BOTTOM + size * 2 * i, size * 4 / 32);
            //((u32*) Display::DISPLAY_BOTTOM)[i] = x | 0x80008000;
        }
        dsi::memory::copyChunks32(Display::DISPLAY_BOTTOM, Display::DISPLAY_TOP, (SCREEN_WIDTH * SCREEN_HEIGHT * 2) >> 5);
        //dsi::bios::vBlankIntrWait();
    }

    if (true) {
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

        return endlessWait();
    }

    if (!isDSiMode()) {
        drawText(5, 5, ARGB16(1, 31, 0, 0), BOTTOM, "Must be used on DSi!");
        return endlessWait();
    } else {
        setCpuClock(true);
    }

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
