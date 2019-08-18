#ifndef DS_LAUNCHER_FONT_H
#define DS_LAUNCHER_FONT_H

#include <nds.h>
#include "constants.h"

//
// this is a really hacky way of getting a font into the program quickly
// without having to write some sort of file parser
//
// because screw that
//

#define BITS_PER_ROW = 5;
#define ROW_MASK = (1 << BITS_PER_ROW) - 1;

#define WIDTH(size) (size & 0xF)
#define HEIGHT(size) ((size >>> 4) & 0xF)

struct Font {
    static unsigned int* LETTERS;
    static unsigned int* SIZES;
public:
    static void init();
};

enum Screen {
    TOP = 0,
    BOTTOM = 1
};

class Display {
    public:
        static Display* TOP;
        static Display* BOTTOM;

        static u16* DISPLAY_TOP;
        static u16* TEMP_DISPLAY_TOP;
        static u16* DISPLAY_BOTTOM;
        static u16* TEMP_DISPLAY_BOTTOM;
        static int CURRENT_FRAME;
        static int KEYS_DOWN;
        static touchPosition TOUCH_POS;
        static bool QUEUED_REDRAW;

        //fields
        Screen screen;
        int row = 0;

        Display(Screen screen)  {
            this->screen = screen;
        }

        void printf(int x, int y, const char* text, ...);
        void print(int x, int y, const char* text);
        void printf(const char* text, ...);
        void print(const char* text);
        void dprintf(int x, int y, const char* text, ...);
        void dprint(int x, int y, const char* text);
};

void drawRect(int x, int y, int w, int h, u16 argb, Screen screen);
int drawText(int x, int y, u16 argb, Screen screen, const char* text);

#endif //DS_LAUNCHER_FONT_H
