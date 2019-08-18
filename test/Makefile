.SUFFIXES:
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

# These set the information text in the nds file
GAME_TITLE     := dsiwifi (test)
GAME_SUBTITLE1 := made by DaPorkchop_
GAME_SUBTITLE2 := https://daporkchop.net

.PHONY: release debug clean all

all: release

release:
	@$(MAKE) -C test_arm9 BUILD=release
	@$(MAKE) -C test_arm7 BUILD=release
	ndstool	-c dsiwifi_test.nds -7 test_arm7/test_arm7.elf -9 test_arm9/test_arm9.elf \
	-b $(GAME_ICON) "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)"

debug:
	@$(MAKE) -C test_arm9 BUILD=debug
	@$(MAKE) -C test_arm7 BUILD=debug

clean:
	@$(MAKE) -C test_arm9 clean
	@$(MAKE) -C test_arm7 clean
	@$(RM) dsiwifi_test.nds test_arm7.elf test_arm9.elf
