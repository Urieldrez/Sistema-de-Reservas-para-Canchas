CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
SRC    = $(wildcard src/*.c)
HDR    = $(wildcard include/*.h)
TARGET = turnos

$(TARGET): $(SRC) $(HDR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)