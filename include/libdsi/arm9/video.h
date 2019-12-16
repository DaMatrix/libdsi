#ifndef LIBDSI_ROOT_VIDEO_H
#define LIBDSI_ROOT_VIDEO_H

#include <libdsi/arm9/dsi9.h>

namespace dsi::video {
    enum Display: u8 {
        DISPLAY_A    = 1,
        DISPLAY_B    = 2,
        DISPLAY_BOTH = DISPLAY_A | DISPLAY_B
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
     * Sets the display currently on top.
     *
     * If the given parameter is not equal to DISPLAY_A or DISPLAY_B, no action will be performed.
     *
     * @param display the display to set as the top display
     */
    void topDisplay(Display display);

    constexpr i32 BRIGHTNESS_WHITE   = 16;
    constexpr i32 BRIGHTNESS_DEFAULT = 0;
    constexpr i32 BRIGHTNESS_BLACK   = -16;

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

    enum DisplayMode: u8 {
        DISPLAY_MODE_WHITE       = 0, //blank white screen
        DISPLAY_MODE_2D          = 1, //normal BG and object layers
        DISPLAY_MODE_BITMAP_VRAM = 2, //engine A only: display a bitmap stored in VRAM
        DISPLAY_MODE_BITMAP_DMA  = 3  //engine A only: display a bitmap stored in main memory, transferred to display using DMA
    };

    /**
     * Sets the display mode for the given display.
     *
     * @param display the display to set the display mode for
     * @param mode    the new display mode to set
     */
    void setDisplayMode(Display display, DisplayMode mode);

    enum BackgroundMode: u8 {
        //why does CLion think this needs continuation indentation

        //for all modes: on engine B, BG0 is always only text
        //                    BG0      BG1      BG2      BG3
            BG_MODE_0 = 0, // Text/3D  Text     Text     Text
            BG_MODE_1 = 1, // Text/3D  Text     Text     Affine
            BG_MODE_2 = 2, // Text/3D  Text     Affine   Affine
            BG_MODE_3 = 3, // Text/3D  Text     Text     Extended
            BG_MODE_4 = 4, // Text/3D  Text     Affine   Extended
            BG_MODE_5 = 5, // Text/3D  Text     Extended Extended
            BG_MODE_6 = 6  // 3D       -        Large    -         (doesn't exist on engine B)
    };

    /**
     * Sets the background mode for the given display.
     *
     * @param display the display to set the background mode for
     * @param mode    the new background mode to set
     */
    void setBackgroundMode(Display display, BackgroundMode mode);

    /**
     * Sets the backdrop color for the given display.
     *
     * The backdrop color is used for pixels that have no objects or other content being displayed on them and would otherwise be transparent.
     *
     * @param display the display to set the backdrop color for
     * @param color   the new backdrop color to set
     */
    void setBackdrop(Display display, argb16 color);
}

#endif //LIBDSI_ROOT_VIDEO_H
