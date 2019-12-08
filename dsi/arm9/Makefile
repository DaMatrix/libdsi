ifneq (build,$(notdir $(CURDIR)))

.PHONY: build clean

build:
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C build -f $(CURDIR)/Makefile all

clean:
	@rm -rf build/ $(TOPDIR)/lib/libdsi9.a

else

SOURCES			:=	$(TOPDIR)/arm9/source $(TOPDIR)/common/source
INCLUDES		:=	$(TOPDIR)/include

CFILES			:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.c'))
CPPFILES		:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.cpp'))
SFILES			:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.S'))
OFILES			:=	$(addsuffix .o,$(subst /,_, $(CFILES)) $(subst /,_, $(CPPFILES)) $(subst /,_, $(SFILES)))

CFLAGS			:=	$(CFLAGS) $(CFLAGS_9) $(foreach dir,$(INCLUDES),-I$(dir)) -Os
CXXFLAGS		:=	$(CFLAGS) $(CXXFLAGS)
ASFLAGS			:=	$(CFLAGS) $(ASFLAGS)
LDFLAGS			:=	$(CFLAGS) $(LDFLAGS)

-include $(subst .c,.d,$(subst /,_, $(CFILES))) $(subst .cpp,.d,$(subst /,_, $(CPPFILES))) $(subst .S,.d,$(subst /,_, $(SFILES)))

all: $(TOPDIR)/lib/libdsi9.a

$(TOPDIR)/lib/libdsi9.a: $(OFILES)
	@echo linking $@
	@rm -f "$@"
	@$(AR) rcs "$@" $(OFILES)

%.c.o:
	$(CC) -MMD -MP -MF $(CURDIR)/$*.d $(CFLAGS) -c $(subst .c.o,.c,$(subst _,/,$@)) -o $@ $(ERROR_FILTER)

%.cpp.o:
	$(CXX) -MMD -MP -MF $(CURDIR)/$*.d $(CXXFLAGS) -c $(subst .cpp.o,.cpp,$(subst _,/,$@)) -o $@ $(ERROR_FILTER)

%.S.o:
	$(CC) -MMD -MP -MF $(CURDIR)/$*.d $(ASFLAGS) -c $(subst .S.o,.S,$(subst _,/,$@)) -o $@ $(ERROR_FILTER)

endif
