
CC=gcc
CFLAGS=-g -Wall

all: main.o processor.o instruction_funcs.o
	gcc -o atari_emulator main.o processor.o instruction_funcs.o

clean:
	rm *.o atari_emulator *.out
