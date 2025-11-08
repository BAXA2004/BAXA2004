CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c17
LDFLAGS = -lm

SRC_DIR = src
INC_DIR = include
TEST_DIR = tests

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

OBJ_FILES = $(SRC_FILES:.c=.o)

TARGET = calc

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

test: $(TEST_FILES)
	$(CC) $(CFLAGS) -I$(INC_DIR) $^ -o test_calc $(LDFLAGS)

clean:
	rm -f $(OBJ_FILES) $(TARGET) test_calc