OUTPUT_FILES	:=	$(addprefix $(CURDIR)/../gcc/arm-none-eabi/lib/,$(wildcard *.ld) $(wildcard *.specs))

.PHONY: clean all

all: $(OUTPUT_FILES) .FORCE

$(CURDIR)/../gcc/arm-none-eabi/lib/%: $(CURDIR)/%
	@cp $< $@

clean:
	@rm -f $(OUTPUT_FILES)

.FORCE:
