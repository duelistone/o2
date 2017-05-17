Othello engine written in C

The engine should be able to compile on any 64-bit system with gcc, though it's only been extensively tested on Ubuntu 16.04. To install just the engine, run "make engine" from this directory.

To also run the tests before compiling the engine, you'll need the "check" C testing framework (and its dependencies). Once you know you have that installed, just run "make" to compile and run the tests and then compile the engine, or "make test" to just compile and run the tests. The tests can take a while to run (currently 21 seconds on my machine).

This program uses the popcount and clz (count leading zeros) instructions very frequently. If your processor doesn't have these instructions, your program may be a lot slower. Also, I have no idea if this will even compile on a 32-bit system, but even if it does it will likely be much slower, since the othello board is represented with two 64-bit integers, and most of the program involves operations on 64 bit integers.
