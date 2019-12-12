OUTPUT_FILES	:=	$(foreach file,$(wildcard *.ld),$(CURDIR)/../gcc/arm-none-eabi/lib/$(file))
OUTPUT_FILES	:=	$(OUTPUT_FILES) $(subst .ld,.specs,$(OUTPUT_FILES))

.PHONY: clean all

all: $(OUTPUT_FILES) .FORCE

$(CURDIR)/../gcc/arm-none-eabi/lib/%.ld: $(CURDIR)/%.ld
	@cp $< $@

%.specs:
	@echo "%rename link_gcc_c_sequence                nosys_link_gcc_c_sequence" > $@
	@echo "" >> $@
	@echo "*nosys_libgloss:" >> $@
	@echo "-lnosys" >> $@
	@echo "" >> $@
	@echo "*nosys_libc:" >> $@
	@echo "%{!specs=nano.specs:-lc} %{specs=nano.specs:-lc_nano}" >> $@
	@echo "" >> $@
	@echo "*link_gcc_c_sequence:" >> $@
	@echo "%(nosys_link_gcc_c_sequence) --start-group %G %(nosys_libc) %(nosys_libgloss) --end-group -T $(subst $(CURDIR)/../gcc/arm-none-eabi/lib/,,$(subst .specs,.ld,$@))" >> $@

clean:
	@rm -f $(OUTPUT_FILES)

.FORCE:
