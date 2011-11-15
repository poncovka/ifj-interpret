CFLAGS=-std=c99 -Wall -pedantic -g -O3
BIN=project
CC=gcc
RM=rm -r

ALL: str.o list.o stack.o table.o parser.o scanner.o interpret.o 
	$(CC) $(FLAGS) -o $(BIN) str.o list.o stack.o table.o parser.o scanner.o interpret.o

clean: 
	$(RM) *.o $(BIN)
