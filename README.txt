Othello engine written in C

The engine should be able to compile on any 64-bit system with gcc. To install just the engine, run "make engine" from this directory.

To also run the tests before compiling the engine, you'll need the "check" C testing framework (and its dependencies). You may have to tweak the Makefile to link the right libraries for it in your system. Once you know you have that installed, just run "make" to compile and run the tests and then compile the engine, or "make test" to just compile and run the tests. The tests can take a while to run (currently 46 seconds on my machine). To run faster tests (by skipping the long endgame searches), just change the definition of FFO_ENDGAME_TESTS in test.c to 0 (or set it to 1 if you'd like these tests and they're not running automatically).

This program uses the popcount and clz (count leading zeros) instructions very frequently. If your processor doesn't have these instructions, your program may be a lot slower. Also, I have no idea if this will even compile on a 32-bit system, but even if it does it will likely be much slower, since the othello board is represented with two 64-bit integers, and most of the program involves operations on 64 bit integers.
