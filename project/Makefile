# Variables
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
