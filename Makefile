#PROFILER=-pg
CFLAGS=-std=c99 -Wall -pedantic -g -O3 $(PROFILER)
BIN=project
CC=gcc
RM=rm -r
MODULS= str.o list.o stack.o table.o parser.o scanner.o expression.o interpret.o binary_tree.o library.o main.o

ALL: $(MODULS)
	$(CC) $(FLAGS) $(PROFILER) -o $(BIN) $(MODULS) -lm

clean: 
	$(RM) *.o $(BIN) # debug.log


