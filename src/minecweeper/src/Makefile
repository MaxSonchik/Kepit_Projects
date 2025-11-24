CC = gcc
CFLAGS = -Wall -I.
LIBS = -lncurses
TARGET = minesweeper

SOURCES = main.c \
          core/game.c \
          tui/core/tui.c \
          tui/input/input.c \
          tui/input/input_handler.c \
          tui/display/display.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o ../build/$(TARGET) $(SOURCES) $(LIBS)

clean:
	rm -f ../build/$(TARGET)

.PHONY: clean