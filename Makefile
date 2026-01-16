NAME = MatMul

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

# all source files include main.c
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Filter out main.c from the list of files needed by tests
COMMON_SRCS = $(filter-out $(SRC_DIR)/main.c, $(SRCS))
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)

# Define object files
APP_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
COMMON_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(COMMON_SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_SRCS))

# Targets
APP_BIN = $(BIN_DIR)/$(NAME)
TEST_BIN = $(BIN_DIR)/test_$(NAME)

# Ensure directories exist
$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))

$(APP_BIN): $(APP_OBJS)
	$(CC) $(APP_OBJS) -o $@

$(TEST_BIN): $(COMMON_OBJS) $(TEST_OBJS)
	$(CC) $(COMMON_OBJS) $(TEST_OBJS) -o $@

# Find object file for each source file (first src, then test)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: $(APP_BIN)
	./$(APP_BIN) matrix/*.txt # add all files in matrix/

bear:
	make clean
	bear -- make

.PHONY: all test clean run bear
