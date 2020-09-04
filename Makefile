

CC = gcc

SOURCE_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

$(shell mkdir -p ${BUILD_DIR})



all: $(OBJECTS)
	gcc $(OBJECTS) -o main


$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) -I$(INCLUDE_DIR) -c $< -o $@


clean:
	rm $(BUILD_DIR)/*.o
