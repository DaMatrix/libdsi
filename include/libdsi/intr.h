#ifndef LIBDSI_INTR_H
#define LIBDSI_INTR_H

namespace dsi::intr {
    /**
     * Initializes the interrupt handler system, clearing any currently registered interrupt handlers.
     */
    void init();

    /**
     * Sets the root interrupt handler (the function that will be called every time an interrupt occurs).
     *
     * The default value for this (as set by init()) delegates interrupts to the corresponding handlers as set by the other methods in this namespace,
     * be aware that using a custom handler may prevent these handlers from working.
     *
     * @param handler the new root handler to use
     */
    void setRootHandler(Void handler);

    void set(u32 interrupt, Void handler);
}

#endif //LIBDSI_INTR_H
