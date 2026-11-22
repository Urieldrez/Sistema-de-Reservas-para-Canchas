CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
SRC    = src/main.c src/reserva.c src/cliente.c src/pila_undo.c src/persistencia.c
HDR    = $(wildcard include/*.h)
TARGET = turnos

$(TARGET): $(SRC) $(HDR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)