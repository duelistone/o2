CC = gcc
CFLAGS = -O3 -Wall -march=native

all: test engine

engine: main.o board.o search.o
	$(CC) $(CFLAGS) -o engine main.o board.o search.o

test: test.o search.o board.o 
	$(CC) $(CFLAGS) -o test test.o search.o board.o -lcheck -lm -lrt -lpthread -lsubunit
	time ./test

board_test: board_test.o board.o search.o
	$(CC) $(CFLAGS) -o board_test board_test.o board.o search.o

test.o: test.c search.h
	$(CC) $(CFLAGS) -c test.c

board.o: board.c board.h defs.h
	$(CC) $(CFLAGS) -c board.c

board_test.o: board_test.c board.h defs.h
	$(CC) $(CFLAGS) -c board_test.c

search.o: search.c search.h board.h defs.h
	$(CC) $(CFLAGS) -c search.c

main.o: main.c search.h board.h defs.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm *.o board_test engine test
