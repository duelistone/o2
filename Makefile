CC=gcc
CFLAGS=-Ofast -Wall -march=native -ffast-math -fprofile-use
PROFILE_FLAGS=-fprofile-generate -pg
LIBS=-lm
TEST_LIBS=-lcheck -lsubunit -lpthread -lrt

all: profile

profile: clean profile-build profile-clean test engine

# Sometimes requires make clean first if previously using profile
simple: main.o board.o search.o hash.o tt.o test.o
	$(CC) $(CFLAGS) -o engine main.o board.o search.o hash.o tt.o $(LIBS)
	$(CC) $(CFLAGS) -o test test.o search.o board.o hash.o tt.o $(LIBS) $(TEST_LIBS)
	strip engine
	strip test

profile-build: main.c board.c search.c hash.c tt.c test.c board.h search.h hash.h tt.h defs.h
	$(CC) $(CFLAGS) $(PROFILE_FLAGS) -o engine main.c board.c search.c hash.c tt.c $(LIBS)
	$(CC) $(CFLAGS) $(PROFILE_FLAGS) -o test test.c search.c board.c hash.c tt.c $(LIBS) $(TEST_LIBS)
	bash -c "time ./test"
	gprof test > profile

engine: main.o board.o search.o hash.o tt.o
	$(CC) $(CFLAGS) -o engine main.o board.o search.o hash.o tt.o -lm
	strip engine

test: test.o board.o search.o hash.o tt.o
	$(CC) $(CFLAGS) -o test test.o search.o board.o hash.o tt.o $(LIBS) $(TEST_LIBS)
	strip test
	bash -c "time ./test"

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

profile-clean:
	-rm *.o engine test

clean:
	-rm *.o *.gcda engine test

.PHONY: all profile clean profile-clean simple
