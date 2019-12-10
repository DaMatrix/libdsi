#include "main.h"

int endlessWait() {
    while (true) { dsi::bios::vBlankIntrWait(); }
    return 0;
}

int main() {
    return endlessWait();
}
