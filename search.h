// search.h

#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "defs.h"
#include <stdint.h>

int alphabeta(u64 black, u64 white, int depth, int alpha, int beta);
int alphabetaMove(u64 black, u64 white, int depth, int alpha, int beta);
int endgameAlphabeta(u64 black, u64 white, u64 lm, int beta);
int endgameAlphabeta62(u64 black, u64 white, u64 lm, int beta);
int endgameAlphabeta63(u64 black, u64 white, int beta);
int endgameAlphabetaMove(u64 black, u64 white, int alpha, int beta);
#endif
