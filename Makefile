

# -- O P E R A T I N G  S Y S T E M -------------------------------------------

# Detect operating system
ifneq ($(OS),Windows_NT)
    override OS := $(shell uname -s)
    ifneq ($(OS),Darwin)
        $(error $(shell echo "Unsupported OS: \033[1;32m$(OS)\033[0m"))
    endif
else
    $(error $(shell echo "Unsupported OS: \033[1;32m$(OS)\033[0m"))
endif


override THREAD := $(shell sysctl -n hw.ncpu)


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
#override MAKEFLAGS += --warn-undefined-variables --no-builtin-rules



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


# -- T A R G E T S ------------------------------------------------------------

# project name
override PROJECT := engine

# main executable
override EXEC := exec_$(PROJECT)

# compile commands for clangd
override COMPILE_COMMANDS = compile_commands.json



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

# compiler flags
override CXXFLAGS := -Wall -Wextra -Werror \
					 -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-private-field \
					 -fno-objc-arc

# linker flags
override LDFLAGS := -framework Metal -framework Foundation -framework Cocoa -framework CoreGraphics -framework MetalKit


# -- S O U R C E S ------------------------------------------------------------

override INCLUDES := $(addprefix -I, $(INCDIR) $(METALDIR))

override SRCS := $(shell find $(SRCDIR) -type f -name '*.cpp')
override OBJS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
override DEPS := $(patsubst $(SRCDIR)/%.cpp, $(DEPDIR)/%.d, $(SRCS))
override JSNS := $(patsubst $(SRCDIR)/%.cpp, $(JSNDIR)/%.json, $(SRCS))

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


all: intro shaders objs $(EXEC) $(COMPILE_COMMANDS)
	@echo "\x1b[32mD O N E\x1b[0m"

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
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp Makefile | $(OBJDIR) $(DEPDIR) $(JSNDIR)
	@echo "compiling $<"
	@$(CXX) $(STD) $(DEBUG) $(CXXFLAGS) $(INCLUDES) $(DEPFLAGS) $(CMPFLAGS) -c $< -o $@

# create directories
$(OBJDIR) $(DEPDIR) $(JSNDIR):
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

