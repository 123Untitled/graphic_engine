# Install Xcode from the Apple App Store.
# Install the command line tools with xcode-select --install. This might do nothing on your machine.
# If xcode-select --print-path prints /Library/Developer/CommandLineTools
# then run sudo xcode-select --switch /Applications/Xcode.app/Contents/Developer.


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




override METAL    := $(shell xcrun --find metal)
override METALLIB := $(shell xcrun --find metallib)


override NAME     := lib.metallib
override SRCS     := shaders.metal loki.metal

override AIRDIR   := air
override AIR      := $(patsubst %.metal, $(AIRDIR)/%.air, $(SRCS))

override MFLAGS   := -std=metal3.0


# -- T A R G E T S ------------------------------------------------------------

.PHONY: all clean fclean re


all: $(NAME)


$(NAME): $(AIR)
	$(METALLIB) $^ -o $@
	@file $(NAME);

$(AIRDIR)/%.air: %.metal Makefile | $(AIRDIR)
	$(METAL) $(MFLAGS) -c $< -o $@

$(AIRDIR):
	@mkdir -pv $@

clean:
	@rm -rvf $(AIRDIR)

fclean: clean
	@rm -rvf $(NAME)

re: fclean all


