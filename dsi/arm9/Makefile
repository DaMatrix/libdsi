ifneq (build,$(notdir $(CURDIR)))
.PHONY: build clean

build:
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C build -f $(CURDIR)/Makefile

clean:
	@rm -rf build/ $(TOPDIR)/lib/libdsi9*

else

SOURCES			:=	$(TOPDIR)/arm9/source $(TOPDIR)/common/source
INCLUDES		:=	$(TOPDIR)/include

ARM9BIN			:=	$(TOPDIR)/lib/libdsi9.a

CFILES			:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.c'))
CFILES_COMP		:=  $(subst /,_, $(CFILES))
CPPFILES		:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.cpp'))
CPPFILES_COMP	:=  $(subst /,_, $(CPPFILES))
SFILES			:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.S'))
SFILES_COMP		:=  $(subst /,_, $(SFILES))

OFILES			:=	$(addsuffix .o,$(CFILES_COMP) $(CPPFILES_COMP) $(SFILES_COMP))
DFILES			:=	$(CPPFILES_COMP:.cpp=.d) $(CFILES_COMP:.c=.d) $(SFILES_COMP:.S=.d)

INCLUDE			:=	$(foreach dir,$(INCLUDES),-I$(dir))

CFLAGS			:=	$(CFLAGS) $(CFLAGS_9) $(INCLUDE) -Os
CXXFLAGS		:=	$(CFLAGS) $(CXXFLAGS)
ASFLAGS			:=	$(CFLAGS) $(ASFLAGS)
LDFLAGS			:=	$(CFLAGS) $(LDFLAGS)

-include $(DFILES)

all: $(ARM9BIN)

# main targets
$(ARM9BIN): $(OFILES) $(CFILES) $(CPPFILES) $(SFILES)
	@echo linking $(ARM9BIN)
	@rm -f "$(ARM9BIN)"
	@$(AR) rcs "$(ARM9BIN)" $(OFILES)

%.c.o: $(CFILES)
	$(CC) -MMD -MP -MF $(CURDIR)/$*.d $(CFLAGS) -c $(subst .c.o,.c,$(subst _,/,$@)) -o $@ $(ERROR_FILTER)

%.cpp.o: $(CPPFILES)
	$(CXX) -MMD -MP -MF $(CURDIR)/$*.d $(CXXFLAGS) -c $(subst .cpp.o,.cpp,$(subst _,/,$@)) -o $@ $(ERROR_FILTER)

%.S.o: $(SFILES)
	$(CC) -MMD -MP -MF $(CURDIR)/$*.d $(ASFLAGS) -c $(subst .S.o,.S,$(subst _,/,$@)) -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
