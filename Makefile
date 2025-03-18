# Compiler
CC = gcc

# Source files
SRC = src/main.c

# Output executable
TARGET = game

# Compiler flags
CFLAGS = -Wall -std=c99 -O2 -I/opt/homebrew/Cellar/raylib/5.5/include
LDFLAGS = -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit

all:
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)