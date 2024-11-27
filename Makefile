CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
SRCDIR = src
BINDIR = bin
SOURCES = $(shell find $(SRCDIR) -name '*.c')
LDFLAGS = -lncurses
FRAMEWORKS = 
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BINDIR)/%.o)
TARGET = $(BINDIR)/out

# # Debug output
# $(info SOURCES = $(SOURCES))
# $(info OBJECTS = $(OBJECTS))

# Default target
all: $(TARGET)

# Linking step
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS) $(FRAMEWORKS)

# Compilation step
$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and binaries
clean:
	rm -f $(BINDIR)/*.o $(TARGET)
