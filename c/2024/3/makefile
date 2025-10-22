# Compiler and flags
CC = clang
CFLAGS = -Wall -ansi -g

# Directories
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/day_03

# Source files
SOURCES = day_03.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Create bin directory
$(BIN_DIR):
	rm -rf $(BIN_DIR)
	mkdir $(BIN_DIR)

# Link object files into executable
$(TARGET): $(BIN_DIR) $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files into bin directory
$(BIN_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean everything
clean:
	rm -rf $(BIN_DIR) $(OBJECTS)

# Rebuild from scratch
rebuild: clean all

.PHONY: all clean rebuild