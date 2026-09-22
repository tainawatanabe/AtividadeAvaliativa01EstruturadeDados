CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

TARGET = labirinto
OBJS = jogo.o labirinto.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

jogo.o: jogo.c labirinto.h
	$(CC) $(CFLAGS) -c jogo.c

labirinto.o: labirinto.c labirinto.h
	$(CC) $(CFLAGS) -c labirinto.c

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
