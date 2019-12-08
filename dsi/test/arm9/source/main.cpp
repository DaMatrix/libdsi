#include "main.h"

static void* src = nullptr;

void vblank() {
    Display::CURRENT_FRAME++;

    scanKeys();
    Display::KEYS_DOWN |= keysDown();
    touchRead(&Display::TOUCH_POS);

    /*for (std::vector<InputHandler>::iterator it = Gui::HANDLERS.begin(); it != Gui::HANDLERS.end(); it++) {
        (*it)(Gui::KEYS_DOWN, &Gui::TOUCH_POS);
    }*/

    if (Display::QUEUED_REDRAW || !(Display::CURRENT_FRAME & 0xF)) {
        dsi::memory::copyChunks32(Display::TEMP_DISPLAY_TOP, Display::DISPLAY_TOP, (SCREEN_WIDTH * SCREEN_HEIGHT * 2) >> 5);
        dsi::memory::copyChunks32(src == nullptr ? Display::TEMP_DISPLAY_BOTTOM : src, Display::DISPLAY_BOTTOM, (SCREEN_WIDTH * SCREEN_HEIGHT * 2) >> 5);

        //GuiExitCode exitCode = Gui::MENU_STACK.back()(keys);

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

extern "C" u32 jeff(void* framebuffer, u32 seed, u32 remaining);

int main() {
    Font::init();
    irqSet(IRQ_VBLANK, vblank);

    if (true)   {
        for (int i = 0; i < 15; i++) swiWaitForVBlank();

        u16 size = (SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(u16)) / sizeof(u32);
        Display::TOP->printf("Allocating %d word buffer...", size);
        auto img = new u32[size];

        Display::TOP->printf("Configuring DMA channel");
        if (false) {
            dsi::dma::Channel* channel = dsi::dma::channel(3);
            channel->src     = img;
            channel->dst     = Display::TEMP_DISPLAY_BOTTOM;
            channel->size    = size * 2;
            channel->control = dsi::dma::ENABLE | dsi::dma::REPEAT | dsi::dma::SRC_FIX | dsi::dma::DST_FIX | dsi::dma::START_VBL;
        } else {
            DMA2_SRC = (u32) img;
            DMA2_DEST = (u32) Display::TEMP_DISPLAY_BOTTOM;
            //DMA2_CR = DMA_ENABLE | DMA_REPEAT | DMA_SRC_FIX | DMA_DST_FIX | DMA_START_VBL | DMA_32_BIT;
        }

        u32 x = 1;
        while (true)    {
            for (auto i = size; i--;) {
                x ^= x << 13;
                x ^= x >> 17;
                x ^= x << 5;
                //((u32*) Display::TEMP_DISPLAY_BOTTOM)[i] = x | 0x80008000;
                img[i] = x | 0x80008000;
            }
            swiWaitForVBlank();
            src = img;

            /*if (!(DMA2_CR & DMA_ENABLE))    {
                Display::TOP->print("DMA channel was completed!");
                endlessWait();
            }*/
            /*Display::TOP->printf("%x %x %d", channel->src, channel->dst, channel->size);
            channel->control = dsi::dma::ENABLE | dsi::dma::START_VBL | dsi::dma::IS_32BIT;
            while (channel->running());
            Display::TOP->printf("%x %x %d", channel->src, channel->dst, channel->size);
            endlessWait();*/
        }
    }

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
