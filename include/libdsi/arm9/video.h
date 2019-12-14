#ifndef LIBDSI_ROOT_VIDEO_H
#define LIBDSI_ROOT_VIDEO_H

#include <libdsi/arm9/dsi9.h>

namespace dsi::video    {
    /**
     * Clears and resets all VRAM banks and mappings to their original state.
     */
    void resetVRAM();
}

#endif //LIBDSI_ROOT_VIDEO_H
