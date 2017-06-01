#ifndef __TT_H__
#define __TT_H__

#include <stdint.h>
#include <string.h>
#include "defs.h"

#define COUNT_NODES 0

// Struct for an endgame transposition table entry
// The structure of the data field is as follows, 
// from MSB to LSB:
// eval     (unsigned 3 bits, using definitions for types of endgame evals)
// In midgame search, we instead have
// eval     (unsigned 16 bits, 2^15 + signed eval)
// alpha    (unsigned 16 bits, 2^15 + signed eval)
// beta     (unsigned 16 bits, 2^15 + signed eval)
// depth    (unsigned 5 bits) 
typedef struct {
    u64 black;
    u64 white;
    u64 data;
} ttEntry;

// Declaration of endgame transposition table
ttEntry endgameTT[ENDGAME_TT_LEN]; // Currently 512 MB, should be initialized to all 0

#if COUNT_COLLISIONS
// For info on hash table usage; this should be removed when not debugging
int collisions[ENDGAME_TT_LEN]; // Should be initialized

// For counting tt hits in debugging
size_t ttHits;
#endif


void initializeTT();

#endif
