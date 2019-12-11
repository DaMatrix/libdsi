ifndef DEVKITARM
$(error "DEVKITARM is not set!")
endif
ifeq ("$(wildcard $(DEVKITARM)/base_rules)","")
$(error "DEVKITARM points to an invalid libdsi installation. Make sure you set it up correctly using setup-environment.sh")
endif

export AR	:=	$(DEVKITARM)/bin/arm-none-eabi-gcc-ar
export CC	:=	$(DEVKITARM)/bin/arm-none-eabi-gcc
export CXX	:=	$(DEVKITARM)/bin/arm-none-eabi-g++
export LD	:=	$(DEVKITARM)/bin/arm-none-eabi-ld

.PHONY: clean all

all: libdsi

libdsi: .FORCE
	@$(MAKE) --no-print-directory -C dsi

clean:
	@$(MAKE) --no-print-directory -C dsi clean

.FORCE:
