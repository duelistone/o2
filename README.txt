Othello engine written in C

The engine should be able to compile on any 64-bit system with gcc. To install just the engine, run "make engine" from this directory, which creates the executable "engine".

To also run the tests before compiling the engine, you'll need the "check" C testing framework (and its dependencies). You may have to tweak the Makefile to link the right libraries for it in your system, though hopefully not. Once you know you have this library installed, just run "make" to compile and run the tests and then compile the engine, or "make test" to just compile and run the tests. The tests can take a while to run (20 seconds to a minute, perhaps slower on some machines).

For an optimized build using gcc profiling, just run "make". This runs the tests, so all the dependencies mentioned above are required, and it may take a while.

This program uses the popcount and clz (count leading zeros) instructions very frequently. If your processor doesn't have these instructions, your program may be a lot slower.

-- Input and Output --

The engine accepts lines of the form "x y t", where (x, y) are the coordinates of the opponent's move (indices starting with 0), and t is the time remaining for the program. As the engine does not currently have time management, the t parameter is currently ignored. A passing move is represented by (-1, -1). If the engine is supposed to go first, the first move should be a passing move.

The first line of each output is of the form "x y", signalling the engine's move. The rest of the lines of output corresponding for each move are currently for debugging purposes and can be ignored.
