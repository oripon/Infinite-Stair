.PHONY: clean

CC = gcc -g -Wall

main: main.c
	$(CC) -o ../game main.c `pkg-config --cflags --libs gtk+-2.0`

clean:
	rm ../game
