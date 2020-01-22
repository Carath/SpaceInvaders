EXE = space_invaders

SRC_DIR = src
OBJ_DIR = obj


CC = gcc
CPPFLAGS =
CFLAGS = -Wall -O2 -std=c99 `sdl2-config --cflags`
LDFLAGS =
LDLIBS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lm


SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


# The following names are not associated with files:
.PHONY: all clean

# All executables to be created:
all: $(EXE)

# Linking the program:
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Compiling other source files:
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Cleaning with 'make clean' the object files:
clean:
	rm -fv $(EXE) $(OBJ_DIR)/*
