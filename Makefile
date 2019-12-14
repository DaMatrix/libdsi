export TARGET			:=	$(shell basename $(CURDIR))
export TOPDIR			:=	$(CURDIR)

export LIBDSI_MAJOR		:=	0
export LIBDSI_MINOR		:=	0
export LIBDSI_REVISION	:=	0
export LIBDSI_SUFFIX	:=	-SNAPSHOT
export VERSION			:=	$(LIBDSI_MAJOR).$(LIBDSI_MINOR).$(LIBDSI_REVISION)$(LIBDSI_SUFFIX)

export CFLAGS		:=	-mthumb -mthumb-interwork -Wall -ffunction-sections -fdata-sections -fomit-frame-pointer -flto=1 -fno-fat-lto-objects -ffast-math -DDSI
export CXXFLAGS		:=	-fno-rtti
export ASFLAGS		:=
export LDFLAGS		:=	-Wl,--nmagic -Wl,-Map,$(notdir $*).map -fuse-linker-plugin

export CFLAGS_7		:=	-DARM7 -march=armv4t -mcpu=arm7tdmi -mtune=arm7tdmi
export LDFLAGS_7	:=	-specs=ds_arm7.specs
export CFLAGS_9		:=	-DARM9 -march=armv5te -mcpu=arm946e-s+nofp -mtune=arm946e-s
export LDFLAGS_9	:=	-specs=dsi_arm9.specs

export AR	:=	$(DEVKITARM)/bin/arm-none-eabi-gcc-ar
export CC	:=	$(DEVKITARM)/bin/arm-none-eabi-gcc
export CXX	:=	$(DEVKITARM)/bin/arm-none-eabi-g++
export LD	:=	$(DEVKITARM)/bin/arm-none-eabi-ld

.PHONY: clean all

all: include/libdsi/version.h arm7 arm9 test

include/libdsi/version.h: Makefile
	@mkdir -p include/libdsi/
	@echo "#ifndef LIBDSI_VERSION_H" > $@
	@echo "#define LIBDSI_VERSION_H" >> $@
	@echo >> $@
	@echo "namespace dsi {" >> $@
	@echo "    constexpr int VERSION_MAJOR    = $(LIBDSI_MAJOR);" >> $@
	@echo "    constexpr int VERSION_MINOR    = $(LIBDSI_MINOR);" >> $@
	@echo "    constexpr int VERSION_REVISION = $(LIBDSI_REVISION);" >> $@
	@echo "    constexpr int VERSION          = (VERSION_MAJOR * 1000 + VERSION_MINOR) * 1000 + VERSION_REVISION;" >> $@
	@echo >> $@
	@echo '    constexpr const char* VERSION_STRING = "'$(LIBDSI_MAJOR).$(LIBDSI_MINOR).$(LIBDSI_REVISION)$(LIBDSI_SUFFIX)'";' >> $@
	@echo "}" >> $@
	@echo >> $@
	@echo "#endif // LIBDSI_VERSION_H" >> $@

arm7: lib include/libdsi/version.h .FORCE
	@$(MAKE) --no-print-directory -C arm7

arm9: lib include/libdsi/version.h .FORCE
	@$(MAKE) --no-print-directory -C arm9

test: arm7 arm9 .FORCE
	@$(MAKE) --no-print-directory -C test

lib:
	@mkdir lib

clean:
	@$(MAKE) --no-print-directory -C arm7 clean
	@$(MAKE) --no-print-directory -C arm9 clean
	@$(MAKE) --no-print-directory -C test clean
	@$(RM) -rf include/libdsi/version.h lib/

.FORCE:
