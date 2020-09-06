
.PHONY: run_tests

CC = gcc

SOURCE_DIR = ../src
INCLUDE_DIR = ../include
BUILD_DIR = ../build

SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

$(shell mkdir -p ${BUILD_DIR})

TESTSD = $(wildcard *.c)
TESTS  = $(notdir $(TESTSD))

TESTS_EXE = $(patsubst %.c, %.exe, $(TESTS))


all: $(TESTS_EXE) $(OBJECTS)

%.exe: %.c $(OBJECTS)
	gcc -I$(INCLUDE_DIR) -I./include $(OBJECTS) src/test_expect.c $< -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) -I$(INCLUDE_DIR) -c $< -o $@


clean:
	rm *.exe
	rm $(BUILD_DIR)/*.o
