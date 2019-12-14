GAME_TITLE     := libdsi (test)
GAME_SUBTITLE1 := made by DaPorkchop_
GAME_SUBTITLE2 := https://daporkchop.net

.PHONY: clean all

all: test.nds

test.nds: $(CURDIR)/arm7/arm7.elf $(CURDIR)/arm9/arm9.elf $(TOPDIR)/lib/libdsi7.a $(TOPDIR)/lib/libdsi9.a
	@ndstool -c $@ -7 $(CURDIR)/arm7/arm7.elf -9 $(CURDIR)/arm9/arm9.elf -b $(DEVKITPRO)/libnds/icon.bmp "$(GAME_TITLE);$(GAME_SUBTITLE1);$(GAME_SUBTITLE2)"

$(CURDIR)/arm7/arm7.elf: $(TOPDIR)/lib/libdsi7.a .FORCE
	@$(MAKE) --no-print-directory -C arm7 build

$(CURDIR)/arm9/arm9.elf: $(TOPDIR)/lib/libdsi9.a .FORCE
	@$(MAKE) --no-print-directory -C arm9 build

clean:
	@$(MAKE) --no-print-directory -C arm7 clean
	@$(MAKE) --no-print-directory -C arm9 clean
	@$(RM) -rf test.nds

.FORCE:
