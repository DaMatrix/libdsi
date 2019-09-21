.SUFFIXES:
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# DATA is a list of directories containing binary files
# all directories are relative to this makefile
BUILD		?=	release
SOURCES		:=	source ../test_common/source
INCLUDES	:=	../../common/include ../test_common/source
DATA		:=

# options for code generation
ARCH	:=	-mthumb-interwork

CFLAGS	:=	-Wall -Ofast \
			-ffunction-sections -fdata-sections -fomit-frame-pointer -ffast-math \
			-march=armv4t -mcpu=arm7tdmi -mtune=arm7tdmi \
			$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM7 -DNDS -DDSI
CXXFLAGS	:=	$(CFLAGS) -fno-rtti
ASFLAGS	:=	$(INCLUDE) $(ARCH) -DARM7

LDFLAGS	=	-specs=ds_arm7.specs $(ARCH) -Wl,--nmagic -Wl,-Map,$(notdir $*).map

LIBS	:=	-lnds7 -lmm7 -ldsi7

# list of directories containing libraries, this must be the top level containing
# include and lib
LIBDIRS	:=	$(LIBNDS) $(TOPDIR)

# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
ifneq ($(BUILD),$(notdir $(CURDIR)))

export ARM7ELF	:=	$(CURDIR)/test_arm7.elf
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) .

CFILES			:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.c'))
CFILES_COMP		:=  $(subst /,_, $(subst ..,_,$(CFILES)))
CPPFILES		:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.cpp'))
CPPFILES_COMP	:=  $(subst /,_, $(subst ..,_,$(CPPFILES)))
SFILES			:=	$(foreach dir,$(SOURCES),$(shell find -L $(dir) -type f -name '*.S'))
SFILES_COMP		:=  $(subst /,_, $(subst ..,_,$(SFILES)))
BINFILES		:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

# use CXX for linking C++ projects, CC for standard C
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
					$(CPPFILES_COMP:.cpp=.cpp.o) $(CFILES_COMP:.c=.c.o) $(SFILES_COMP:.S=.S.o)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean ...
	@rm -fr $(BUILD) *.elf *.nds* *.bin

else

all: $(ARM7ELF)

# main targets
$(ARM7ELF)	:	$(OFILES) .FORCE
	@echo linking $(notdir $@)
	$(LD)  $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

# you need a rule like this for each extension you use as binary data
%.bin.o	:	%.bin
	@echo $(notdir $<)
	@$(bin2o)

%.c.o: $(CFILES)
	@echo "$(notdir $<) -> $(notdir $@)"
	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c ../$(subst .c.o,.c,$(subst _,/,$(subst __,../,$@))) -S -o $@.S $(ERROR_FILTER)
	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c ../$(subst .c.o,.c,$(subst _,/,$(subst __,../,$@))) -o $@ $(ERROR_FILTER)

%.cpp.o: $(CPPFILES)
	@echo "$(notdir $<) -> $(notdir $@)"
	$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) -c ../$(subst .cpp.o,.cpp,$(subst _,/,$(subst __,../,$@))) -S -o $@.S $(ERROR_FILTER)
	$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) -c ../$(subst .cpp.o,.cpp,$(subst _,/,$(subst __,../,$@))) -o $@ $(ERROR_FILTER)

%.S.o: $(SFILES)
	@echo "$(notdir $<) -> $(notdir $@)"
	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(ASFLAGS) -c ../$(subst .S.o,.S,$(subst _,/,$(subst __,../,$@))) -o $@ $(ERROR_FILTER)

.FORCE:

-include $(DEPSDIR)/*.d

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
