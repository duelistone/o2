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
simple: main.o board.o search.o hash.o tt.o test.o nn.o
	$(CC) $(CFLAGS_NO_PROFILE) -o engine main.o board.o search.o hash.o tt.o nn.o -lm
	$(CC) $(CFLAGS_NO_PROFILE) -o test test.o search.o board.o hash.o tt.o -lcheck -lsubunit nn.o -lm -lrt -lpthread
	strip engine
	strip test

engine: main.o board.o search.o hash.o tt.o nn.o
	$(CC) $(CFLAGS) -o engine main.o board.o search.o hash.o tt.o nn.o -lm
	strip engine

test: test.o search.o board.o hash.o tt.o nn.o
	$(CC) $(CFLAGS) -o test test.o search.o board.o hash.o tt.o nn.o -lcheck -lsubunit -lm -lrt -lpthread 
	strip test
	bash -c "time ./test"

test.o: test.c search.h defs.h tt.h nn.h hash.h
	$(CC) $(CFLAGS) -c test.c

board.o: board.c eval_code.c board.h defs.h 
	$(CC) $(CFLAGS) -c board.c

search.o: search.c search.h board.h tt.h nn.h hash.h defs.h 
	$(CC) $(CFLAGS) -c search.c

hash.o: hash.c hash.h defs.h
	$(CC) $(CFLAGS) -c hash.c

tt.o: tt.c tt.h nn.h defs.h
	$(CC) $(CFLAGS) -c tt.c

nn.o: nn.c nn.h defs.h
	$(CC) $(CFLAGS) -c nn.c

main.o: main.c search.h board.h defs.h tt.h nn.h
	$(CC) $(CFLAGS) -c main.c

timing_test.o: timing_test.c defs.h board.h search.h
	$(CC) $(CFLAGS) -c timing_test.c

timing_test: timing_test.o search.o board.o tt.o nn.o hash.o
	$(CC) $(CFLAGS) -o timing_test timing_test.o search.o board.o tt.o nn.o hash.o

clean:
	-rm *.o *.gcda engine test

.PHONY: all profile clean simple
