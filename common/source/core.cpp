#include <dsi.h>

using namespace dsi;

//allocate space in bss for an internal stack used during init and error handling
//TODO: this doesn't work, figure out why
//__attribute__((section(".bss"))) u8 __internal_dummy_stack[1024 * 3];
