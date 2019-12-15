#ifndef LIBDSI_ROOT_VIDEO_H
#define LIBDSI_ROOT_VIDEO_H

#include <libdsi/arm9/dsi9.h>

namespace dsi::video {
    enum Display: u8 {
        DISPLAY_TOP = 1, //actually engine A
        DISPLAY_BOTTOM = 2, //actually engine B
        DISPLAY_BOTH = DISPLAY_TOP | DISPLAY_BOTTOM
    };

    /**
     * Clears and resets all VRAM banks and mappings to their original state.
     */
    void resetVRAM();

    /**
     * Swaps the top and bottom displays.
     */
    void swapDisplays();

    /**
     * Resets the brightness level of the given display to default.
     *
     * @param display the display to reset the brightness level on
     */
    void resetBrightness(Display display);

    /**
     * Sets the brightness level of the given display.
     *
     * @param display    the display to set the brightness on
     * @param brightness the new brightness level. 0: default, -16: black, 16: white
     */
    void setBrightness(Display display, i32 brightness);
}

#endif //LIBDSI_ROOT_VIDEO_H
