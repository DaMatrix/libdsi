.SUFFIXES:
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

export TARGET	:=	$(shell basename $(CURDIR))
export TOPDIR	:=	$(CURDIR)

# These set the information text in the nds file
GAME_TITLE     := dsiwifi (test)
GAME_SUBTITLE1 := made by DaPorkchop_
GAME_SUBTITLE2 := https://daporkchop.net

export DSIWIFI_MAJOR	:= 0
export DSIWIFI_MINOR	:= 0
export DSIWIFI_REVISION	:= 0
export DSIWIFI_SUFFIX	:= -SNAPSHOT
VERSION	:=	$(DSIWIFI_MAJOR).$(DSIWIFI_MINOR).$(DSIWIFI_REVISION)$(DSIWIFI_SUFFIX)

include $(DEVKITARM)/ds_rules

.PHONY: checkarm7 checkarm9 clean

# main targets
all: common/include/dsiwifi/version.h $(TARGET).nds

common/include/dsiwifi/version.h : Makefile
	@mkdir -p common/include/dsiwifi/
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

checkarm7: common/include/dsiwifi/version.h
	$(MAKE) -C arm7

checkarm9: common/include/dsiwifi/version.h
	$(MAKE) -C arm9

$(TARGET).nds	: $(NITRO_FILES) checkarm7 checkarm9
	ndstool	-c $(TARGET).nds -7 arm7/arm7.elf -9 arm9/arm9.elf \
	-b $(GAME_ICON) "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)"

clean:
	$(MAKE) -C arm9 clean
	$(MAKE) -C arm7 clean
	rm -f $(TARGET).nds arm7/arm7.elf arm9/arm9.elf
