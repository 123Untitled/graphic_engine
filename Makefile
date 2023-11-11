

# -- S E T T I N G S ----------------------------------------------------------

# set default target
.DEFAULT_GOAL := all

# use one shell for all commands
.ONESHELL:

# delete intermediate files on error
.DELETE_ON_ERROR:

# set shell program
override SHELL := $(shell which zsh)

# set shell flags
.SHELLFLAGS := --no-rcs --no-globalrcs --errexit --no-unset -c -o pipefail

# set make flags
override MAKEFLAGS += --warn-undefined-variables --no-builtin-rules


# -- T A R G E T S ------------------------------------------------------------

# project name
override PROJECT := engine

# main executable
override EXEC := exec_$(PROJECT)

# compile commands for clangd
override COMPILE_COMMANDS = compile_commands.json


# -- E X T E R N A L  D E P E N D E N C I E S ---------------------------------

# xns library name
override LIBNAME := xns

# xns library directory
override XNSDIR := $(LIBNAME)

# xns library flags
override LXNS := -L$(XNSDIR) -l$(LIBNAME)



# -- D I R E C T O R I E S ----------------------------------------------------

# build directory
override BLDDIR := build

# source directory
override SRCDIR := sources

# include directory
override INCDIR := includes

# object directory
override OBJDIR := $(BLDDIR)/objects

# dependency directory
override DEPDIR := $(BLDDIR)/dependencies

# json directory
override JSNDIR := $(BLDDIR)/json

# metal directory
override METALDIR := metal

# shader directory
override SHADIR := shaders





# -- C O M P I L E R  S E T T I N G S -----------------------------------------

# compiler
override CXX := clang++

# compiler standard
override STD := -std=c++2a

# compiler optimization
override OPT := -O0

# debug flags
override DEBUG := -g

# address sanitizer flags
override ASANFLAGS := -fsanitize=address

# warning scope
override CXXFLAGS := -Wall -Wextra

# warning impact
override CXXFLAGS += -Werror

# objective-c
override CXXFLAGS += -fno-objc-arc

# standard respect
override CXXFLAGS += -Weffc++
#-Wpedantic

# unused suppression
override CXXFLAGS += -Wno-unused -Wno-unused-variable -Wno-unused-parameter \
					 -Wno-unused-private-field -Wno-unused-local-typedef \
					 -Wno-unused-but-set-variable -Wno-unused-function 

# optimization
override CXXFLAGS += -Winline

# type conversion
#override CXXFLAGS += -Wconversion -Wsign-conversion -Wfloat-conversion -Wnarrowing

# shadowing
#override CXXFLAGS += -Wshadow


# linker flags
override LDFLAGS := -framework Metal \
					-framework Foundation \
					-framework Cocoa \
					-framework CoreGraphics \
					-framework MetalKit \
					$(LXNS)



# -- S O U R C E S ------------------------------------------------------------


# get all source files
override SRCS := $(shell find $(SRCDIR) -type f -name '*.cpp')

# pattern substitution for object files
override OBJS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o,    $(SRCS))

# pattern substitution for dependency files
override DEPS := $(patsubst $(OBJDIR)/%.o,   $(DEPDIR)/%.d,    $(OBJS))

# pattern substitution for json files
override JSNS := $(patsubst $(SRCDIR)/%.cpp, $(JSNDIR)/%.json, $(SRCS))


# get all sub source directories
override SUBSRCDIR := $(sort $(dir $(SRCS)))

# pattern substitution for sub directories
override SUBOBJDIR := $(SUBSRCDIR:$(SRCDIR)/%=$(OBJDIR)/%)
override SUBDEPDIR := $(SUBSRCDIR:$(SRCDIR)/%=$(DEPDIR)/%)
override SUBJSNDIR := $(SUBSRCDIR:$(SRCDIR)/%=$(JSNDIR)/%)

# get all sub include directories
override SUBINCDIR := $(shell find $(INCDIR) -type d)

# include flags
override INCLUDES := $(addprefix -I, $(SUBINCDIR) $(METALDIR) $(XNSDIR))



# dependency flags
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

# compile commands flags
CMPFLAGS = -MJ $(JSNDIR)/$*.json




# -- P H O N Y  T A R G E T S -------------------------------------------------

.PHONY: all clean fclean re intro shaders


intro:
	@echo "\x1b[32mM A K E\x1b[0m"


shaders:
	@$(MAKE) --silent -C $(SHADIR)


all: intro $(XNSDIR) shaders objs $(EXEC) $(COMPILE_COMMANDS)
	@echo "\x1b[32mD O N E\x1b[0m"


$(XNSDIR):
	@echo '\x1b[32m[xns]\x1b[0m' "Checking external dependencies\n";
	@git clone git@github.com:123Untitled/xns.git xns_tmp;
	@cd xns_tmp && ./make.sh release && tools/single_header.sh && mv libxns.a xns/ && mv xns/ .. && cd .. && rm -rf xns_tmp;

# executable
$(EXEC): $(OBJS)
	@echo "linking $@"
	@$(CXX) $(LDFLAGS) $^ -o $@
	@file $(EXEC)

# launch threads
objs:
	@$(MAKE) --silent -j8 $(OBJS)

# compilation
-include $(DEPS)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp Makefile | $(SUBOBJDIR) $(SUBDEPDIR) $(SUBJSNDIR)
	@echo "compiling $<"
	@$(CXX) $(STD) $(DEBUG) $(CXXFLAGS) $(INCLUDES) $(DEPFLAGS) $(CMPFLAGS) -c $< -o $@

# create directories
$(SUBOBJDIR) $(SUBDEPDIR) $(SUBJSNDIR):
	@mkdir -pv $@

# compile commands
$(COMPILE_COMMANDS): $(JSNS)
	@echo "creating $@"
	@echo "[\n"$$(cat $(JSNS) | sed '$$s/,\s*$$//')"\n]" | jq > $@


clean:
	@rm -rvf $(BLDDIR) $(COMPILE_COMMANDS) .cache
	@$(MAKE) --silent -C $(SHADIR) clean

fclean: clean
	@rm -rvf $(EXEC)
	@$(MAKE) --silent -C $(SHADIR) fclean

re: fclean all

