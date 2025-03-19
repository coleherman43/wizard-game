# Compiler
CC = gcc

# Source files
SRC = src/main.c src/projectile.c

# Output executable
TARGET = game

# Compiler flags
CFLAGS = -Wall -std=c99 -O2 -I/opt/homebrew/Cellar/raylib/5.5/include
LDFLAGS = -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -lm

# Build target
all:
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# Clean target
clean:
	rm -f $(TARGET)