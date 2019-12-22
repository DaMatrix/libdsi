#include <dsi.h>

using namespace dsi;

#ifdef ARM9
//allocate space in bss for an internal stack used during init and error handling
u8 __internal_dummy_stack[1024 * 3];
#endif
