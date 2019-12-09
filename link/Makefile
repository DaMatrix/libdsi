OUTPUT_FILES	:=	$(subst .S,.o,$(wildcard *.S)) $(subst .ld,.specs,$(wildcard *.ld))

.PHONY: clean all

all: $(OUTPUT_FILES) .FORCE

%.o: %.S
	@[ -d thumb ] || mkdir -p thumb
	@$(CC) -x assembler-with-cpp -marm -c $< -o $@
	@$(CC) -x assembler-with-cpp -mthumb -c $< -o thumb/$@

%.specs:
	@echo "%rename link old_link" > $@
	@echo "" >> $@
	@echo "*link:" >> $@
	@echo "%(old_link) -T $(CURDIR)/$(subst .specs,.ld,$@)%s --gc-sections" >> $@
	@echo "" >> $@
	@echo "*startfile:" >> $@
	@echo "$(subst .specs,,$@)-crt0%O%s $(CURDIR)/crti%O%s $(CURDIR)/crtbegin%O%s" >> $@

clean:
	@rm -rf $(OUTPUT_FILES) thumb/

.FORCE:
