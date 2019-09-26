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
SOURCES		:=	source ../common/source
INCLUDES	:=	../include
DATA		:=

# options for code generation
ARCH	:=	-mthumb-interwork

CFLAGS	:=	-Wall -Os \
			-ffunction-sections -fdata-sections -fomit-frame-pointer -ffast-math \
			-march=armv4t -mcpu=arm7tdmi -mtune=arm7tdmi \
			$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM7 -DNDS -DDSI
CXXFLAGS	:=	$(CFLAGS) -fno-rtti
ASFLAGS	:=	$(INCLUDE) $(ARCH) -DARM7

LDFLAGS	=	-specs=ds_arm7.specs $(ARCH) -Wl,--nmagic -Wl,-Map,$(notdir $*).map

LIBS	:=

# list of directories containing libraries, this must be the top level containing
# include and lib
LIBDIRS	:=	$(LIBNDS)
 
  
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
ifneq ($(BUILD),$(notdir $(CURDIR)))
 
ifneq ($(BUILD),debug)
export ARM7BIN	:=	$(TOPDIR)/lib/libdsi7.a
else
export ARM7BIN	:=	$(TOPDIR)/lib/libdsi7d.a
CFLAGS	+=	-DSGIP_DEBUG
CXXFLAGS	+=	-DSGIP_DEBUG
endif

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

export LD	:=	$(CXX)

export OFILES	:=	$(addsuffix .o,$(BINFILES)) $(CPPFILES_COMP:.cpp=.cpp.o) $(CFILES_COMP:.c=.c.o) $(SFILES_COMP:.S=.S.o)
export DFILES	:=	$(CPPFILES_COMP:.cpp=.d) $(CFILES_COMP:.c=.d) $(SFILES_COMP:.S=.d)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@rm -rf debug release $(TOPDIR)/lib/liblibdsi7*

else

-include $(DFILES) $(DEPSDIR)/*.d

all: $(ARM7BIN)

# main targets
$(ARM7BIN)	:	$(OFILES)
	@echo linking $(ARM7BIN)
	@rm -f "$(ARM7BIN)"
	@$(AR) rcs "$(ARM7BIN)" $(OFILES)

%.bin.o	:	%.bin
	@$(bin2o)

%.c.o: $(CFILES)
	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c ../$(subst .c.o,.c,$(subst _,/,$(subst __,../,$@))) -S -o $@.S $(ERROR_FILTER)
	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c ../$(subst .c.o,.c,$(subst _,/,$(subst __,../,$@))) -o $@ $(ERROR_FILTER)

%.cpp.o: $(CPPFILES)
	$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) -c ../$(subst .cpp.o,.cpp,$(subst _,/,$(subst __,../,$@))) -S -o $@.S $(ERROR_FILTER)
	$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) -c ../$(subst .cpp.o,.cpp,$(subst _,/,$(subst __,../,$@))) -o $@ $(ERROR_FILTER)

%.S.o: $(SFILES)
	$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(ASFLAGS) -c ../$(subst .S.o,.S,$(subst _,/,$(subst __,../,$@))) -o $@ $(ERROR_FILTER)

.FORCE:

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
