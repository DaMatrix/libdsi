#ifndef LIBDSI_HOME
#$(error "LIBDSI_HOME is not set!")
#endif
#ifeq ("$(wildcard $(LIBDSI_HOME)/gcc/README.md)","")
#$(error "LIBDSI_HOME points to an invalid libdsi installation. Make sure you set it up correctly using setup-environment.sh")
#endif

export TARGET		:=	$(shell basename $(CURDIR))
export TOPDIR		:=	$(CURDIR)

export LIBDSI_HOME	?=	$(CURDIR)

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
export LDFLAGS_7	:=	-specs=$(LIBDSI_HOME)/link/dsi-arm7.specs
export CFLAGS_9		:=	-DARM9 -march=armv5te -mcpu=arm946e-s+nofp -mtune=arm946e-s
export LDFLAGS_9	:=	-specs=$(LIBDSI_HOME)/link/dsi-arm9.specs

ifeq ("false","true")
export AR	:=	$(LIBDSI_HOME)/gcc/bin/arm-none-eabi-gcc-ar
export CC	:=	$(LIBDSI_HOME)/gcc/bin/arm-none-eabi-gcc
export CXX	:=	$(LIBDSI_HOME)/gcc/bin/arm-none-eabi-g++
export LD	:=	$(LIBDSI_HOME)/gcc/bin/arm-none-eabi-ld
else
export AR	:=	$(DEVKITARM)/bin/arm-none-eabi-gcc-ar
export CC	:=	$(DEVKITARM)/bin/arm-none-eabi-gcc
export CXX	:=	$(DEVKITARM)/bin/arm-none-eabi-g++
export LD	:=	$(DEVKITARM)/bin/arm-none-eabi-ld
endif

XPACK_VERSION	:=	"9.2.1-1.1"
XPACK_URL		:=	"https://github.com/xpack-dev-tools/arm-none-eabi-gcc-xpack/releases/download/v$(XPACK_VERSION)/xpack-arm-none-eabi-gcc-$(XPACK_VERSION)-linux-x64.tar.gz"

.PHONY: clean all

all: include/libdsi/version.h link arm7 arm9 test

include/libdsi/version.h: .FORCE
	@[ -d "$(CURDIR)/include/libdsi/" ] || mkdir -p "$(CURDIR)/include/libdsi/"
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

arm7: link lib include/libdsi/version.h $(LIBDSI_HOME)/gcc/README.md .FORCE
	@$(MAKE) --no-print-directory -C arm7

arm9: link lib include/libdsi/version.h $(LIBDSI_HOME)/gcc/README.md .FORCE
	@$(MAKE) --no-print-directory -C arm9

test: arm7 arm9 .FORCE
	@$(MAKE) --no-print-directory -C test

link: .FORCE
	@$(MAKE) --no-print-directory -C link

$(LIBDSI_HOME)/gcc/README.md:
	@if [ -d "$(CURDIR)/temp/" ]; then rm -rf "$(CURDIR)/temp/"; fi
	@echo "Downloading xPack GCC v$(XPACK_VERSION)"
	@rm -rf "$(CURDIR)/gcc/"
	@[ -d "$(CURDIR)/temp/" ] || mkdir -p "$(CURDIR)/temp/"
	@curl -o - -L "$(XPACK_URL)" | tar zxf - -C "$(CURDIR)/temp/" && mv "$(CURDIR)/temp/xpack-arm-none-eabi-gcc-$(XPACK_VERSION)/" "$(CURDIR)/gcc/"

lib:
	@mkdir lib

clean: .FORCE
	@$(MAKE) --no-print-directory -C arm7 clean
	@$(MAKE) --no-print-directory -C arm9 clean
	@$(MAKE) --no-print-directory -C test clean
	@$(MAKE) --no-print-directory -C link clean
	@$(RM) -rf include/libdsi/version.h lib/

.FORCE:
	@if [ -d "$(CURDIR)/temp/" ]; then rm -rf "$(CURDIR)/temp/"; fi
