CC=gcc
CFLAGS=-g -Wall

all: main.o processor.o instruction_funcs.o
	gcc -o 8080_emulator main.o processor.o instruction_funcs.o 

clean:
	rm *.o 8080_emulator *.out
