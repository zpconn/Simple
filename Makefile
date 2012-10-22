CC = gcc
CFLAGS = -Wall -g -ansi -pedantic

all: simple

simple: driver.o loader.o error.o variable_map.o command_parser.o executor.o data_structures.o helper.o expression.o
	$(CC) $(CFLAGS) -o simple driver.o loader.o error.o variable_map.o command_parser.o executor.o data_structures.o helper.o expression.o

driver.o: driver.c error.h loader.h
	$(CC) $(CFLAGS) -c driver.c

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c

loader.o: loader.c loader.h error.h helper.h
	$(CC) $(CFLAGS) -c loader.c

executor.o: executor.c executor.h loader.h error.h command_parser.c command_parser.h
	$(CC) $(CFLAGS) -c executor.c

command_parser.o: command_parser.c command_parser.h loader.h variable_map.h helper.h
	$(CC) $(CFLAGS) -c command_parser.c

variable_map.o: variable_map.c variable_map.h loader.h error.h
	$(CC) $(CFLAGS) -c variable_map.c

data_structures.o: data_structures.c data_structures.h
	$(CC) $(CFLAGS) -c data_structures.c

helper.o: helper.c helper.h
	$(CC) $(CFLAGS) -c helper.c

expression.o: expression.c expression.h data_structures.h variable_map.h loader.h helper.h error.h
	$(CC) $(CFLAGS) -c expression.c

clean:
	rm -f simple *.o *~
