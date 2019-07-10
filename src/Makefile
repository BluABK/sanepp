# Variables
NAME :=sane
OBJS =main.o
CXX = g++

# Release preset
#CFLAGS=-O3 -Wall -std=c99
# Debug preset
CFLAGS=-O0 -g3 -Wall -Wextra -std=c99 -DDEBUG

# Declare "phony targets"
.PHONY: all clean

all: sane

$(NAME): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(NAME) $(OBJS) $(LDLIBS)

# '@' Runs it in silent mode, and '-' will supress errors.
clean:
	@- $(RM) $(OBJS)

distclean: clean
	@- $(RM) $(NAME)
