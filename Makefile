CC = gcc
CFLAGS = -Ofast -Wall -march=native -ffast-math -fprofile-use

all: test engine

profile:
	make clean
	make -f Makefile-generate-profile
	make clean
	make all

engine: main.o board.o search.o hash.o tt.o hash.o
	$(CC) $(CFLAGS) -o engine main.o board.o search.o hash.o tt.o -lm
	strip engine

test: test.o search.o board.o hash.o tt.o
	$(CC) $(CFLAGS) -o test test.o search.o board.o hash.o tt.o -lcheck -lm -lrt -lpthread 
	strip test
	time ./test

test.o: test.c search.h defs.h tt.h hash.h
	$(CC) $(CFLAGS) -c test.c

board.o: board.c eval_code.c board.h defs.h
	$(CC) $(CFLAGS) -c board.c

search.o: search.c search.h board.h tt.h hash.h defs.h 
	$(CC) $(CFLAGS) -c search.c

hash.o: hash.c hash.h defs.h
	$(CC) $(CFLAGS) -c hash.c

tt.o: tt.c tt.h defs.h
	$(CC) $(CFLAGS) -c tt.c

main.o: main.c search.h board.h defs.h tt.h
	$(CC) $(CFLAGS) -c main.c

clean:
	-rm *.o engine test

.PHONY: all profile clean
