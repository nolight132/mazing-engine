CC = gcc
CFLAGS = -Wall -Wextra -I./src
SRCDIR = src
BINDIR = bin
SOURCES = $(wildcard $(SRCDIR)/*.c)
LDFLAGS = -lncurses
FRAMEWORKS = 
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BINDIR)/%.o)
TARGET = $(BINDIR)/out

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