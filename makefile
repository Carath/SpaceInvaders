##########################################################
# Settings:

# Executable name:
EXE = spaceinvaders.exe

# Source and objects files location:
SRC_DIR = src
OBJ_DIR = obj

# Creates the OBJ_DIR directory, if necessary:
$(shell mkdir -p $(OBJ_DIR))

##########################################################
# Libraries:

# Remove this line if SDL2 isn't installed or to be used:
GRAPHIC_LIB = SDL2

ifeq ($(GRAPHIC_LIB), SDL2)
	GRAPHIC_FLAGS = `sdl2-config --cflags`
	GRAPHIC_LINKS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf
endif

##########################################################
# Compiler options:

# N.B: gcc for C, g++ for C++, alternative: clang.
CC = gcc
CPPFLAGS =
CFLAGS = -std=c99 -Wall -O2 $(GRAPHIC_FLAGS)
LDFLAGS =
LDLIBS = $(GRAPHIC_LINKS) -lm

##########################################################
# Compiling rules:

# The following names are not associated with files:
.PHONY: all clean

# All executables to be created:
all: $(EXE)

# Sources and objects files:
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Linking the program:
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Compiling the source files:
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

##########################################################
# Cleaning with 'make clean' the object files:

clean:
	rm -fv $(EXE) $(OBJ_DIR)/*
