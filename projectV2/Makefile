CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lMLV -lm

SRC_DIR = src
OBJ_DIR = bin
DOC_DIR = doc

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE = $(OBJ_DIR)/quadtree

all: $(EXECUTABLE) doc

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(DOC_DIR)/html

doc: $(DOC_DIR)/Doxyfile
	cd $(DOC_DIR) && doxygen Doxyfile

.PHONY: all clean doc
