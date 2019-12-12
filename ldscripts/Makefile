LD_FILES		:=	$(wildcard *.ld)
SPECS_FILES		:=	$(wildcard *.specs)
OUTPUT_FILES	:=	$(addprefix $(CURDIR)/../gcc/arm-none-eabi/lib/,$(LD_FILES) $(SPECS_FILES))
OUTPUT_FILES	:=	$(OUTPUT_FILES) $(foreach a,arm7 arm9,$(CURDIR)/../gcc/arm-none-eabi/lib/dsi-$(a)-libgloss.o)

.PHONY: clean all

all: $(OUTPUT_FILES) .FORCE

$(CURDIR)/../gcc/arm-none-eabi/lib/%.ld: $(CURDIR)/%.ld
	@cp $< $@

$(CURDIR)/../gcc/arm-none-eabi/lib/%.specs: $(CURDIR)/%.specs
	@cp $< $@

$(CURDIR)/../gcc/arm-none-eabi/lib/dsi-arm%-libgloss.o: $(CURDIR)/effectively-libgloss.S
	@$(CC) -x assembler-with-cpp -marm -I$(CURDIR)/../dsi/include -DARM$(patsubst $(CURDIR)/../gcc/arm-none-eabi/lib/dsi-arm%-libgloss.o,%,$@) -c $< -o $@

clean:
	@rm -f $(OUTPUT_FILES)

.FORCE:
