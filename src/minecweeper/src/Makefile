CC = gcc
CFLAGS = -Wall -I.
TARGET = minesweeper
SOURCES = main.c core/game.c ui/menu.c ui/display.c data/config.c

$(TARGET): $(SOURCES)
		$(CC) $(CFLAGS) -o ../build/$(TARGET) $(SOURCES)

clean:
		rm -f ../build/$(TARGET)

.PHONY: clean