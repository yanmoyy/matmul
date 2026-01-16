# Compiler and flags
CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2

# Directories
OBJ_DIR = obj
BIN_DIR = bin

# Target executable names
TARGET     = $(BIN_DIR)/MatMul.exe
TEST_TARGET = $(BIN_DIR)/test_array.exe

# Source files
MAIN_SRCS   = main.c
TEST_SRCS   = test_array.c array.c
ARRAY_SRCS  = array.c

# Object files (automatically placed in obj/)
MAIN_OBJS   = $(patsubst %.c,$(OBJ_DIR)/%.o,$(MAIN_SRCS))
TEST_OBJS   = $(patsubst %.c,$(OBJ_DIR)/%.o,$(TEST_SRCS))
ARRAY_OBJS  = $(patsubst %.c,$(OBJ_DIR)/%.o,$(ARRAY_SRCS))

# Default target
all: $(TARGET)

# Build main program
$(TARGET): $(MAIN_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Build test program (links array.o too)
$(TEST_TARGET): $(TEST_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Rule to compile .c → .o into obj/ directory
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

# Clean everything
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: $(TARGET)
	./$(TARGET) matrix/A.txt matrix/B.txt

# Run tests
test: $(TEST_TARGET)
	@echo "Running tests..."
	./$(TEST_TARGET) 

# Phony targets (not real files)
.PHONY: all clean run test

# Prevent make from trying to treat these as files
.PRECIOUS: $(OBJ_DIR)/%.o
