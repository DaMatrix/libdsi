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

int main() {
    Font::init();
    irqSet(IRQ_VBLANK, vblank);

    /*Display::HANDLERS.push_back([](int keys, touchPosition *touch) -> void {
        if (keys & KEY_START) {
            Display::TOP->print("Closing socket...");
            Socket::INSTANCE.close();
            //exit(0);
        }
    });*/

    Display::TOP->print("Waiting...");
    while (Display::CURRENT_FRAME < 200) swiWaitForVBlank();

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
    } catch (const char *e) {
        drawText(5, 5, ARGB16(1, 31, 0, 0), BOTTOM, e);
    } catch (void *e) {
        drawText(5, 5, ARGB16(1, 31, 0, 0), BOTTOM, "Exception");
    }
    //Socket::INSTANCE.close();
    while (true) {
        swiWaitForVBlank();
    }
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
    char *result = new char[snprintf(nullptr, 0, format, args) + 1];
    sprintf(result, format, args);
    return result;
}
