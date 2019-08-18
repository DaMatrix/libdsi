.SUFFIXES:
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

export TARGET	:=	$(shell basename $(CURDIR))
export TOPDIR	:=	$(CURDIR)

export DSIWIFI_MAJOR	:= 0
export DSIWIFI_MINOR	:= 0
export DSIWIFI_REVISION	:= 0
export DSIWIFI_SUFFIX	:= -SNAPSHOT
VERSION	:=	$(DSIWIFI_MAJOR).$(DSIWIFI_MINOR).$(DSIWIFI_REVISION)$(DSIWIFI_SUFFIX)

.PHONY: release debug clean all

all: include/dsiwifi/version.h release

include/dsiwifi/version.h : Makefile
	@mkdir -p include/dsiwifi/
	@echo "#ifndef DSIWIFI_VERSION_H" > $@
	@echo "#define DSIWIFI_VERSION_H" >> $@
	@echo >> $@
	@echo "#define DSIWIFI_MAJOR    $(DSIWIFI_MAJOR)" >> $@
	@echo "#define DSIWIFI_MINOR    $(DSIWIFI_MINOR)" >> $@
	@echo "#define DSIWIFI_REVISION $(DSIWIFI_REVISION)" >> $@
	@echo "#define DSIWIFI_SUFFIX   $(DSIWIFI_SUFFIX)" >> $@
	@echo >> $@
	@echo '#define DSWIFI_VERSION "'$(DSIWIFI_MAJOR).$(DSIWIFI_MINOR).$(DSIWIFI_REVISION)$(DSIWIFI_SUFFIX)'"' >> $@
	@echo >> $@
	@echo "#endif // DSIWIFI_VERSION_H" >> $@

release: lib
	@$(MAKE) -C arm9 BUILD=release
	@$(MAKE) -C arm7 BUILD=release
	@$(MAKE) -C test BUILD=release

debug: lib
	@$(MAKE) -C arm9 BUILD=debug
	@$(MAKE) -C arm7 BUILD=debug
	@$(MAKE) -C test BUILD=debug

lib:
	@mkdir lib

clean:
	@$(MAKE) -C arm9 clean
	@$(MAKE) -C arm7 clean
	@$(MAKE) -C test clean
	@$(RM) -r include/dsiwifi/version.h lib

