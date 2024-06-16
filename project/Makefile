CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lMLV
SRCDIR = src
INCDIR = include
BINDIR = bin
OBJDIR = $(BINDIR)/obj
TARGET = $(BINDIR)/quadtree

# Source and object files
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS = $(wildcard $(INCDIR)/*.h)

# Rules
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -o $@ -c $<

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean

# CC = gcc
# CFLAGS = -Wall -Wextra -Iinclude
# LDFLAGS = -lMLV -lm

# SRC_DIR = src
# OBJ_DIR = bin

# SOURCES = $(wildcard $(SRC_DIR)/*.c)
# OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
# EXECUTABLE = $(OBJ_DIR)/quadtree

# all: $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS)
# 	$(CC) -o $@ $^ $(LDFLAGS)

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
# 	$(CC) $(CFLAGS) -o $@ -c $<

# $(OBJ_DIR):
# 	mkdir -p $(OBJ_DIR)

# clean:
# 	rm -rf $(OBJ_DIR)

# .PHONY: all clean