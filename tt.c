#include "tt.h"

void initializeTT() {
    // Initializing global arrays
    memset(endgameTT, 0, ENDGAME_TT_LEN * sizeof(ttEntry));
    #if COUNT_COLLISIONS
    memset(collisions, 0, ENDGAME_TT_LEN * sizeof(int));
    #endif
    ttHits = 0;
}
