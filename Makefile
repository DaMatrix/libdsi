.SUFFIXES:
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

export TARGET	:=	$(shell basename $(CURDIR))
export TOPDIR	:=	$(CURDIR)

export LIBDSI_MAJOR		:= 0
export LIBDSI_MINOR		:= 0
export LIBDSI_REVISION	:= 0
export LIBDSI_SUFFIX	:= -SNAPSHOT
VERSION	:=	$(LIBDSI_MAJOR).$(LIBDSI_MINOR).$(LIBDSI_REVISION)$(LIBDSI_SUFFIX)

.PHONY: release debug clean all

all: include/libdsi/version.h release

include/libdsi/version.h : Makefile
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

release: lib include/libdsi/version.h
	@$(MAKE) --no-print-directory -C arm9 BUILD=release
	@$(MAKE) --no-print-directory -C arm7 BUILD=release
	@$(MAKE) --no-print-directory -C test BUILD=release

debug: lib include/libdsi/version.h
	@$(MAKE) --no-print-directory -C arm9 BUILD=debug
	@$(MAKE) --no-print-directory -C arm7 BUILD=debug
	@$(MAKE) --no-print-directory -C test BUILD=debug

lib:
	@mkdir lib

clean:
	@$(MAKE) --no-print-directory -C arm9 clean
	@$(MAKE) --no-print-directory -C arm7 clean
	@$(MAKE) --no-print-directory -C test clean
	@$(RM) -r include/libdsi/version.h lib

