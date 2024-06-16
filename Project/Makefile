CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lMLV -lm
SRC_DIR = src
OBJ_DIR = bin
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
EXEC = ./bin/quadtree

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC)

.PHONY: clean
