ifndef LIBDSI_HOME
$(error "LIBDSI_HOME is not set!")
endif
ifeq ("$(wildcard $(LIBDSI_HOME)/gcc/README.md)","")
$(error "LIBDSI_HOME points to an invalid libdsi installation. Make sure you set it up correctly using setup-environment.sh")
endif

export AR	:=	$(LIBDSI_HOME)/gcc/bin/arm-none-eabi-gcc-ar
export CC	:=	$(LIBDSI_HOME)/gcc/bin/arm-none-eabi-gcc
export CXX	:=	$(LIBDSI_HOME)/gcc/bin/arm-none-eabi-g++
export LD	:=	$(LIBDSI_HOME)/gcc/bin/arm-none-eabi-ld

.PHONY: clean all

all: ldscripts libdsi

libdsi: ldscripts .FORCE
	@$(MAKE) --no-print-directory -C dsi

ldscripts: .FORCE
	@$(MAKE) --no-print-directory -C ldscripts

clean:
	@$(MAKE) --no-print-directory -C dsi clean
	@$(MAKE) --no-print-directory -C ldscripts clean

.FORCE:
