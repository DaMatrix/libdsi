ifneq (build,$(notdir $(CURDIR)))
.PHONY: build clean
export PROJDIR	:=	$(CURDIR)

build:
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C build -f $(CURDIR)/Makefile all

clean:
	@rm -rf build/ $(PROJDIR)/arm9.elf

else

SOURCES			:=	$(PROJDIR)/source $(TOPDIR)/test/common/source
INCLUDES		:=	$(TOPDIR)/include $(TOPDIR)/test/common/source $(DEVKITPRO)/libnds/include/

CFILES			:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.c'))
CPPFILES		:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.cpp'))
SFILES			:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.S'))
OFILES			:=	$(addsuffix .o,$(subst /,_, $(CFILES)) $(subst /,_, $(CPPFILES)) $(subst /,_, $(SFILES)))

CFLAGS			:=	$(CFLAGS) $(CFLAGS_9) $(foreach dir,$(INCLUDES),-I$(dir)) -O3
CXXFLAGS		:=	$(CFLAGS) $(CXXFLAGS)
ASFLAGS			:=	$(CFLAGS) $(ASFLAGS)
LDFLAGS			:=	$(LDFLAGS_9) $(CFLAGS) $(LDFLAGS)
LIBS			:=	-L$(TOPDIR)/lib/ -ldsi9 -L$(DEVKITPRO)/libnds/lib/

-include $(subst .c,.d,$(subst /,_, $(CFILES))) $(subst .cpp,.d,$(subst /,_, $(CPPFILES))) $(subst .S,.d,$(subst /,_, $(SFILES)))

all: $(PROJDIR)/arm9.elf

$(PROJDIR)/arm9.elf: $(OFILES) $(TOPDIR)/lib/libdsi9.a
	@echo linking $@
	@rm -f $@
	@$(CXX) $(LDFLAGS) $(OFILES) $(LIBS) -o $@

%.c.o:
	$(CC) -MMD -MP -MF $(CURDIR)/$*.d $(CFLAGS) -c $(subst .c.o,.c,$(subst _,/,$@)) -o $@ $(ERROR_FILTER)

%.cpp.o:
	$(CXX) -MMD -MP -MF $(CURDIR)/$*.d $(CXXFLAGS) -c $(subst .cpp.o,.cpp,$(subst _,/,$@)) -o $@ $(ERROR_FILTER)

%.S.o:
	$(CC) -MMD -MP -MF $(CURDIR)/$*.d $(ASFLAGS) -c $(subst .S.o,.S,$(subst _,/,$@)) -o $@ $(ERROR_FILTER)

endif
