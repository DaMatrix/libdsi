.SUFFIXES:
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

export TARGET	:=	$(shell basename $(CURDIR))
export TOPDIR	:=	$(CURDIR)

export LIBDSI_MAJOR	:= 0
export LIBDSI_MINOR	:= 0
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
	@echo "#define LIBDSI_MAJOR    $(LIBDSI_MAJOR)" >> $@
	@echo "#define LIBDSI_MINOR    $(LIBDSI_MINOR)" >> $@
	@echo "#define LIBDSI_REVISION $(LIBDSI_REVISION)" >> $@
	@echo "#define LIBDSI_SUFFIX   $(LIBDSI_SUFFIX)" >> $@
	@echo >> $@
	@echo '#define DSWIFI_VERSION "'$(LIBDSI_MAJOR).$(LIBDSI_MINOR).$(LIBDSI_REVISION)$(LIBDSI_SUFFIX)'"' >> $@
	@echo >> $@
	@echo "#endif // LIBDSI_VERSION_H" >> $@

release: lib include/libdsi/version.h
	@$(MAKE) -C arm9 BUILD=release
	@$(MAKE) -C arm7 BUILD=release
	@$(MAKE) -C test BUILD=release

debug: lib include/libdsi/version.h
	@$(MAKE) -C arm9 BUILD=debug
	@$(MAKE) -C arm7 BUILD=debug
	@$(MAKE) -C test BUILD=debug

lib:
	@mkdir lib

clean:
	@$(MAKE) -C arm9 clean
	@$(MAKE) -C arm7 clean
	@$(MAKE) -C test clean
	@$(RM) -r include/libdsi/version.h lib

