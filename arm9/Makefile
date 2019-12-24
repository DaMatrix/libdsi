ifneq (build,$(notdir $(CURDIR)))
.PHONY: build clean
export PROJDIR	:=	$(CURDIR)

build:
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C build -f $(CURDIR)/Makefile all

clean:
	@rm -rf build/ $(TOPDIR)/lib/libdsi9.a

else

SOURCES			:=	$(PROJDIR)/source $(TOPDIR)/common/source
INCLUDES		:=	$(TOPDIR)/include $(DEVKITPRO)/libnds/include/

CFILES			:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.c'))
CPPFILES		:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.cpp'))
SFILES			:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.S'))
OFILES			:=	$(addsuffix .o,$(subst /,_,$(CFILES) $(CPPFILES) $(SFILES)))

CFLAGS			:=	$(CFLAGS) $(CFLAGS_9) $(addprefix -I,$(INCLUDES)) -Os
CXXFLAGS		:=	$(CFLAGS) $(CXXFLAGS)
ASFLAGS			:=	$(CFLAGS) $(ASFLAGS)
LDFLAGS			:=	$(CFLAGS) $(LDFLAGS)

-include $(subst .c,.d,$(subst /,_, $(CFILES))) $(subst .cpp,.d,$(subst /,_, $(CPPFILES))) $(subst .S,.d,$(subst /,_, $(SFILES)))

all: $(TOPDIR)/lib/libdsi9.a

$(TOPDIR)/lib/libdsi9.a: $(OFILES)
	@echo linking $@
	@rm -f $@
	@$(AR) rcs $@ $(OFILES)

%.c.o:
	$(CC) -MMD -MP -MF $(CURDIR)/$*.c.d $(CFLAGS) -c $(subst .c.o,.c,$(subst _,/,$@)) -o $@ $(ERROR_FILTER)

%.cpp.o:
	$(CXX) -MMD -MP -MF $(CURDIR)/$*.cpp.d $(CXXFLAGS) -c $(subst .cpp.o,.cpp,$(subst _,/,$@)) -o $@ $(ERROR_FILTER)

%.S.o:
	$(CC) -MMD -MP -MF $(CURDIR)/$*.S.d $(ASFLAGS) -c $(subst .S.o,.S,$(subst _,/,$@)) -o $@ $(ERROR_FILTER)

endif
