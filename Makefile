# Compiler
CC = gcc

# Source files
SRC = src/main.c src/projectile.c src/enemy.c

# Object files
OBJ = $(SRC:.c=.o)

# Output executable
TARGET = game

# Compiler flags
CFLAGS = -Wall -std=c99 -O2 -I/opt/homebrew/Cellar/raylib/5.5/include -I./src
LDFLAGS = -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -lm

# Build target
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile each source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(TARGET) $(OBJ)