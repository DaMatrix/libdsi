OUTFILES	:=	$(subst .S,.o,$(wildcard *.S)) dsi-arm7.specs dsi-arm9.specs

.PHONY: all clean

all: $(OUTFILES)

clean:
	@rm -f $(OUTFILES)

%7.o: %7.S
	@$(CC) -x assembler-with-cpp -marm -I$(TOPDIR)/include -DDSI -DARM7 -c $< -o $@

%9.o: %9.S
	@$(CC) -x assembler-with-cpp -marm -I$(TOPDIR)/include -DDSI -DARM9 -c $< -o $@

dsi-arm7.specs: .FORCE
	@echo "%rename link                old_link" > $@
	@echo "" >> $@
	@echo "*link:" >> $@
	@echo "%(old_link) -T $(DEVKITARM)/arm-none-eabi/lib/ds_arm7.ld%s --gc-sections" >> $@
	@echo "" >> $@
	@echo "*startfile:" >> $@
	@echo "$(LIBDSI_HOME)/link/crt0-dsi-arm7%O%s crti%O%s crtbegin%O%s" >> $@

dsi-arm9.specs: .FORCE
	@echo "%rename link                old_link" > $@
	@echo "" >> $@
	@echo "*link:" >> $@
	@echo "%(old_link) -T $(LIBDSI_HOME)/link/dsi-arm9.ld%s --gc-sections" >> $@
	@echo "" >> $@
	@echo "*startfile:" >> $@
	@echo "$(LIBDSI_HOME)/link/crt0-dsi-arm9%O%s crti%O%s crtbegin%O%s" >> $@

.FORCE:
