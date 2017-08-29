CC = gcc
CFLAGS_NO_PROFILE = -Ofast -Wall -march=native -ffast-math
CFLAGS = $(CFLAGS_NO_PROFILE) -fprofile-use

all: profile

profile:
	make clean
	make -f Makefile-generate-profile
	make clean
	make test
	make engine

# Requires make clean first if previously using profile
simple: main.o board.o search.o hash.o tt.o test.o
	$(CC) $(CFLAGS_NO_PROFILE) -o engine main.o board.o search.o hash.o tt.o -lm
	$(CC) $(CFLAGS_NO_PROFILE) -o test test.o search.o board.o hash.o tt.o -lcheck -lm -lrt -lpthread 
	strip engine
	strip test

engine: main.o board.o search.o hash.o tt.o
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

.PHONY: all profile clean simple
