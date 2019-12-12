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

MODULES			:=	dsi gcc ldscripts

XPACK_VERSION	:=	"9.2.1-1.1"
XPACK_URL		:=	"https://github.com/xpack-dev-tools/arm-none-eabi-gcc-xpack/releases/download/v$(XPACK_VERSION)/xpack-arm-none-eabi-gcc-$(XPACK_VERSION)-linux-x64.tar.gz"

.PHONY: clean all

all: $(addprefix build-,$(MODULES))

build-%: .FORCE
	@[ ! -f $(patsubst build-%,%,$@)/Makefile ] || $(MAKE) --no-print-directory -C $(patsubst build-%,%,$@)

build-dsi: build-ldscripts build-gcc
build-ldscripts: build-gcc
build-gcc: $(LIBDSI_HOME)/gcc/README.md

$(LIBDSI_HOME)/gcc/README.md:
	@echo "Downloading xPack GCC v$(XPACK_VERSION)"
	@rm -rf "$(CURDIR)/gcc/"
	@[ -d "$(CURDIR)/temp/" ] || mkdir -p "$(CURDIR)/temp/"
	@curl -o - -L "$(XPACK_URL)" | tar zxf - -C "$(CURDIR)/temp/" && mv "$(CURDIR)/temp/xpack-arm-none-eabi-gcc-$(XPACK_VERSION)/" "$(CURDIR)/gcc/"

clean: $(addprefix clean-,$(MODULES))

clean-%: .FORCE
	@[ ! -f $(patsubst clean-%,%,$@)/Makefile ] || $(MAKE) --no-print-directory -C $(patsubst clean-%,%,$@) clean

.FORCE:
	@if [ -d "$(CURDIR)/temp/" ]; then rm -rf "$(CURDIR)/temp/"; fi
