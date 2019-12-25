#include <dsi.h>

extern dsi::u32 __exectionVectors[8];

namespace dsi::except {
    //addresses of exception handlers in BIOS, but directly to the handlers
    static u32 biosExceptionAddresses[] = {
        0xFFFF0110,
        0xFFFF00C0,
        0xFFFF0298,
        0xFFFF00C0,
        0xFFFF00C0,
        0xFFFF00C0,
        0xFFFF0274,
        0xFFFF00C0
    };

    u32 encodeBranch(Exception exception, void* target) {
        return encodeBranch((void*) (exception << 2), target);
    }

    u32 encodeBranch(void* source, void* target) {
        u32 distance = ((u32) target) - ((u32) source + 8);
        DSI_ASSERT((distance & 0x80000000 ? ~distance : distance) < 0x7FFFFF, "Target out of range!")

        u32 instruction = 0;
        instruction |= 0xE0000000; //condition field (0xE: always)
        instruction |= 0x0A000000; //this is a branch
        instruction |= (distance >> 2) & 0xFFFFFF;
        return instruction;
    }

    void delegateToBIOS(Exception exception)    {
        __exectionVectors[exception] = encodeBranch(exception, (void*) biosExceptionAddresses[exception]);
    }
}
