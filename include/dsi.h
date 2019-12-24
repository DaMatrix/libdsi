#ifndef LIBDSI_H
#define LIBDSI_H

#include <libdsi/common.h>

#ifdef ARM9
#include <libdsi/arm9/dsi9.h>
#else
#ifdef ARM7
#include <libdsi/arm7/dsi7.h>
#else
#error Either ARM7 or ARM9 must be defined!
#endif
#endif

#endif //LIBDSI_H
