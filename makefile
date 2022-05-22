# Project structure
BUILD_DIR ?= bin
SOURCE_DIR ?= src

# Project parameters
PROJECT = pgen
SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# Compiler parameters
CC = gcc
CFLAGS ?=
CFLAGS += -std=c99 -pedantic -g -Wall -Werror

all: clean $(PROJECT)

$(PROJECT) : $(OBJECTS)
	$(CC) $< -o $(BUILD_DIR)/$@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	@rm -vrf $(BUILD_DIR)
