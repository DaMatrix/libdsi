#!/bin/bash

rm test_arm9.elf

$DEVKITARM/bin/arm-none-eabi-g++ \
-specs=dsi_arm9.specs -Wall \
-Wl,--nmagic -Wl,-Map,.map \
-Ofast \
-march=armv5te -mtune=arm946e-s -mthumb -mthumb-interwork \
-fomit-frame-pointer -ffast-math -fno-rtti \
-L/media/daporkchop/TooMuchStuff/PortableIDE/NDS/libnds/lib \
-DARM9 -DNDS -DDSI \
-I../../libnds/include -Itest_common/source/ -I../include/ \
-o test_arm9.elf \
test_arm9/source/*.cpp ../arm9/source/*.cpp ../common/source/* \
-lnds9

ndstool	-c libdsi_test.nds -7 test_arm7/test_arm7.elf -9 test_arm9/test_arm9.elf \
-b /opt/devkitpro/libnds/icon.bmp "libdsi (test);made by DaPorkchop_;https://daporkchop.net"
