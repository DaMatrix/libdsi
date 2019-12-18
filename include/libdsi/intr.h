#ifndef LIBDSI_INTR_H
#define LIBDSI_INTR_H

namespace dsi::intr {
    /**
     * Initializes the interrupt handler system, clearing any currently registered interrupt handlers.
     */
    void init();
}

#endif //LIBDSI_INTR_H
