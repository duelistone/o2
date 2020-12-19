#ifndef __BOARD_H__
#define __BOARD_H__
// board.h

// Functions helpful for working with boards.
// A board consists of a white bitboard and a black bitboard,
// along with an optional zobrist hash.
// However, no struct will be defined here...two or three arguments 
// should be passed instead.

#include <stdint.h>
#include "defs.h"

u64 findLegalMoves(u64 black, u64 white);
u64 doMove(u64 black, u64 white, u8 move);
u64 frontier(u64 black, u64 white);
double frontierScore(u64 black, u64 white, u64 poison);
u8 findRegions(u64 empty, u64 *regions);
u64 findMargin(u64 region);
int eval(u64 black, u64 white, u64 lm);
void printEval(u64 black, u64 white);
void printEval2(u64 black, u64 white);
void printBoard(u64 black, u64 white);
void printBoard2(u64 black, u64 white);

#endif
