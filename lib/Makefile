.PHONY: clean

mak = gcc -g -Wall
dir = ../include

libMakeQueue.a: MakeQueue.o
	ar rv libMakeQueue.a MakeQueue.o

MakeQueue.o: MakeQueue.c ../include/MakeQueue.h
	$(mak) -c MakeQueue.c $(dir)

clean:
	rm *o libMakeQueue.a
