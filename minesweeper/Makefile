# Makefile for Minesweeper Game
# For Windows (MinGW) and Linux (if ported)

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = 

# Detect OS
ifeq ($(OS),Windows_NT)
    # Windows
    TARGET = minesweeper.exe
    RM = del /Q
    MKDIR = if not exist
else
    # Linux/Unix
    TARGET = minesweeper
    RM = rm -f
    MKDIR = mkdir -p
endif

# Source files
SOURCES = main.c \
          common.c \
          display.c \
          input.c \
          menu.c \
          game.c \
          settings.c \
          store.c \
          statistics.c \
          achievements.c \
          abilities.c \
          theme.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo.
	@echo Build successful! Run $(TARGET) to play the game.

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	$(RM) $(OBJECTS) $(TARGET)
	@echo Clean complete.

# Rebuild everything
rebuild: clean all

# Help target
help:
	@echo Minesweeper Game Makefile
	@echo.
	@echo Available targets:
	@echo   make          - Build the game
	@echo   make clean    - Remove build artifacts
	@echo   make rebuild - Clean and rebuild
	@echo   make help     - Show this help message
	@echo.
	@echo For Windows: Run 'make' or 'mingw32-make'
	@echo For Linux: Run 'make' (after porting Windows API calls)

.PHONY: all clean rebuild help
