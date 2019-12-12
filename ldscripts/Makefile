OUTPUT_FILES	:=	$(foreach file,$(wildcard *.ld) $(wildcard *.specs),$(CURDIR)/../gcc/arm-none-eabi/lib/$(file))

.PHONY: clean all

all: $(OUTPUT_FILES) .FORCE

$(CURDIR)/../gcc/arm-none-eabi/lib/%: $(CURDIR)/%
	@cp $< $@

clean:
	@rm -f $(OUTPUT_FILES)

.FORCE:
