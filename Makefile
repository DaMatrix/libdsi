#ifndef LIBDSI_HOME
#$(error "LIBDSI_HOME is not set!")
#endif
#ifeq ("$(wildcard $(LIBDSI_HOME)/gcc/README.md)","")
#$(error "LIBDSI_HOME points to an invalid libdsi installation. Make sure you set it up correctly using setup-environment.sh")
#endif

export LIBDSI_HOME	?=	$(CURDIR)

export AR		:=	$(LIBDSI_HOME)/gcc/bin/arm-none-eabi-gcc-ar
export CC		:=	$(LIBDSI_HOME)/gcc/bin/arm-none-eabi-gcc
export CXX		:=	$(LIBDSI_HOME)/gcc/bin/arm-none-eabi-g++
export LD		:=	$(LIBDSI_HOME)/gcc/bin/arm-none-eabi-ld

XPACK_VERSION	:=	"9.2.1-1.1"
XPACK_URL		:=	"https://github.com/xpack-dev-tools/arm-none-eabi-gcc-xpack/releases/download/v$(XPACK_VERSION)/xpack-arm-none-eabi-gcc-$(XPACK_VERSION)-linux-x64.tar.gz"

.PHONY: clean all

all: ldscripts libdsi gcc

libdsi: ldscripts gcc .FORCE
	@$(MAKE) --no-print-directory -C dsi

ldscripts: gcc .FORCE
	@$(MAKE) --no-print-directory -C ldscripts

gcc: $(LIBDSI_HOME)/gcc/README.md .FORCE

$(LIBDSI_HOME)/gcc/README.md:
	@echo "Downloading xPack GCC v$(XPACK_VERSION)"
	@rm -rf "$(CURDIR)/gcc/"
	@[ -d "$(CURDIR)/temp/" ] || mkdir -p "$(CURDIR)/temp/"
	@curl -o - -L "$(XPACK_URL)" | tar zxf - -C "$(CURDIR)/temp/" && mv "$(CURDIR)/temp/xpack-arm-none-eabi-gcc-$(XPACK_VERSION)/" "$(CURDIR)/gcc/"

clean:
	@$(MAKE) --no-print-directory -C dsi clean
	@$(MAKE) --no-print-directory -C ldscripts clean

.FORCE:
	@if [ -d "$(CURDIR)/temp/" ]; then rm -rf "$(CURDIR)/temp/"; fi
