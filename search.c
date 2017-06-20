// search.c

#include "search.h"

// Comparison function for fastest first heuristic
// Look at the endgame alphabeta function to see the array
// this is working on.
int ffComp(const void *a, const void *b) {
    // Want to compare the popcounts of the last of the three u64's.
    return PC(*((u64 *) a + 2)) - PC(*((u64 *) b + 2));
}

int alphabeta(u64 black, u64 white, int depth, int alpha, int beta) {
    // Deal with no legal moves possibility
    u64 lm = findLegalMoves(black, white);
    if (lm == 0) {
        lm = findLegalMoves(white, black);
        if (lm == 0) {
            return DD(black, white) * MAX_EVAL;
        }
        return -alphabeta(white, black, depth - 1, -beta, -alpha);
    }

    // Save initial board state
    u64 originalBlack = black;
    u64 originalWhite = white;

    // Return static evaluation at depth 0 or less (yes, less is possible due to lm == 0 case)
    if (depth <= 0) {
        return eval(black, white);
    }
    else if (depth == 1) {
        // Main alphabeta algorithm
        while (lm && (alpha < beta)) {
            // Extract next legal move and make the move
            int index = CLZ(lm);
            lm ^= BIT(index);
            black = doMove(originalBlack, originalWhite, index);
            white = originalWhite & ~black;

            // Get eval
            int result = -eval(white, black);
            alpha = (result > alpha) ? result : alpha;
        }
        return alpha;
    }
        
    // Save original alpha and beta
    int originalAlpha = alpha;
    int originalBeta = beta;
    
    // Check midgame transposition table (same endgameTT is used for both to save space)
    #if COUNT_COLLISIONS
    int canCollide = 1; // To count collisions later
    #endif
    u32 hash;
    if (depth > STOP_USING_TT_DEPTH) {
        hash = boardHash(originalBlack, originalWhite);
        if (endgameTT[hash].black == originalBlack && endgameTT[hash].white == originalWhite) {
            #if COUNT_COLLISIONS
            canCollide = 0;
            ttHits++;
            #endif
            u64 data = endgameTT[hash].data;
            int d = EXTRACT_MIDGAME_TT_DEPTH(data);
            if (depth == d) {
                int ee = EXTRACT_MIDGAME_TT_EVAL(data);
                int a = EXTRACT_MIDGAME_TT_ALPHA(data);
                int b = EXTRACT_MIDGAME_TT_BETA(data);
                if (a < ee && ee < b) {
                    return ee;
                }
                else if (ee >= b && b >= beta) {
                    return beta;
                }
                else if (ee >= b) {
                    alpha = b;
                }
                else if (ee <= a && a <= alpha) {
                    return alpha;
                }
                else if (ee <= a) {
                    beta = a;
                }
            }
        }
    }

    // Iterative deepening
    if (depth > 4) {
        // Shallow alphabeta search
        int shallowResult = alphabetaMove(black, white, 2, alpha, beta);
        
        // Get first move to try
        u8 firstMove = EXTRACT_MOVE(shallowResult);

        // Try first move
        lm ^= BIT(firstMove);
        black = doMove(originalBlack, originalWhite, firstMove);
        white = originalWhite & ~black;

        // Recursive call and update alpha
        int result = -alphabeta(white, black, depth - 1, -beta, -alpha);
        if (result > alpha) alpha = result;
    }
    else if (depth > 2) {
        u8 firstMove = EXTRACT_MOVE(alphabetaMove(black, white, 1, alpha, beta));

        // Try first move
        lm ^= BIT(firstMove);
        black = doMove(originalBlack, originalWhite, firstMove);
        white = originalWhite & ~black;

        // Recursive call and update alpha
        int result = -alphabeta(white, black, depth - 1, -beta, -alpha);
        if (result > alpha) alpha = result;
    }

    // Fastest first heuristic while depth is large enough
    if (depth > 3) {
        // The legal moves will be ordered so that moves giving the opponent
        // less mobility appear first.

        // Array for moves, boards, and the number of legal moves each gives
        // Each "entry" is 2 * 8 (black and white boards) + 8 (score) = 3 * 8 bytes
        u64 arr[32 * 3];

        // Index to keep track of place in arrays
        u8 moveIndex = 0;

        // Total number of legal moves
        u8 numLegalMoves = PC(lm);

        // Loop through legal moves
        while (lm) {
            // Extract move
            u8 square = CLZ(lm);
            lm ^= BIT(square);

            // Make move
            black = doMove(originalBlack, originalWhite, square);
            white = originalWhite & ~black;

            // Store board, move, and number of opponent's legal moves
            arr[3 * moveIndex] = black;
            arr[3 * moveIndex + 1] = white;
            arr[3 * moveIndex + 2] = PC(findLegalMoves(white, black));

            // Update moveIndex
            moveIndex++;
        }

        // Sort
        qsort(arr, numLegalMoves, 3 * sizeof(u64), ffComp);

        // Main alphabeta algorithm
        for (size_t i = 0; alpha < beta && i < numLegalMoves; i++) {
            // Recursive call and update alpha
            int result = -alphabeta(arr[3 * i + 1], arr[3 * i], depth - 1, -alpha - 1, -alpha);
            if (result > alpha) {
                if (result >= beta) {
                    alpha = beta;
                    goto save_to_hash;
                }
                alpha = -alphabeta(arr[3 * i + 1], arr[3 * i], depth - 1, -beta, -result);
            }
        }
    }
    else {
        // Main alphabeta algorithm
        while (lm && (alpha < beta)) {
            // Extract next legal move and make the move
            int index = CLZ(lm);
            lm ^= BIT(index);
            black = doMove(originalBlack, originalWhite, index);
            white = originalWhite & ~black;

            // Recursive call, pvs, and update alpha
            int result = -alphabeta(white, black, depth - 1, -alpha - 1, -alpha);
            if (result > alpha) {
                // Failed high
                if (result >= beta) {
                    alpha = beta;
                    goto save_to_hash;
                }
                alpha = -alphabeta(white, black, depth - 1, -beta, -result);
            }
        }
    }

    save_to_hash:

    // Save results to hash
    if (depth > STOP_USING_TT_DEPTH) {
        #if 0
        if (endgameTT[hash].black 
                                  #if COUNT_COLLISIONS
                                  && canCollide
                                  #endif
                                  ) {
            #if COUNT_COLLISIONS
            collisions[hash]++;
            #endif
            // Replacement scheme is always replace
        }
        #endif
            endgameTT[hash].black = originalBlack;
            endgameTT[hash].white = originalWhite;
            endgameTT[hash].data = CONSTRUCT_MIDGAME_TT_DATA(alpha, originalAlpha, originalBeta, depth);
    }

    return alpha;
}

int alphabetaMove(u64 black, u64 white, int depth, int alpha, int beta) {
    // Deal with no legal moves possibility
    u64 lm = findLegalMoves(black, white);
    if (lm == 0) {
        lm = findLegalMoves(white, black);
        if (lm == 0) {
            return ((DD(black, white) * MAX_EVAL) * 256) | NULL_MOVE;
        }
        return (-alphabeta(white, black, depth - 1, -beta, -alpha) * 256) | NULL_MOVE;
    }

    // Return static evaluation at depth 0 or less (yes, less is possible due to lm == 0 case)
    if (depth <= 0) {
        return (eval(black, white) * 256) | NULL_MOVE; // No move should be given for depth 0 search
    }
    
    // Save initial board state
    u64 originalBlack = black;
    u64 originalWhite = white;

    // Declare move
    u8 move = NULL_MOVE;

    // Iterative deepening
    if (depth > 1) {
        move = EXTRACT_MOVE(alphabetaMove(originalBlack, originalWhite, depth - 1, alpha, beta));

        // Try first move
        lm ^= BIT(move);
        black = doMove(originalBlack, originalWhite, move);
        white = originalWhite & ~black;

        // Recursive call and update alpha
        int result = -alphabeta(white, black, depth - 1, -beta, -alpha);
        if (result > alpha) alpha = result;
    }
    else {
        move = CLZ(lm);
        lm ^= BIT(move);
        black = doMove(originalBlack, originalWhite, move);
        white = originalWhite & ~black;
        int result = -alphabeta(white, black, depth - 1, -beta, -alpha);
        alpha = (alpha > result) ? alpha : result;
    }
    
    // Main alphabeta algorithm
    while (lm && (alpha < beta)) {
        // Extract next legal move and make the move
        u8 index = CLZ(lm);
        lm ^= BIT(index);
        black = doMove(originalBlack, originalWhite, index);
        white = originalWhite & ~black;

        // Recursive call, pvs, and update alpha
        int result = -alphabeta(white, black, depth - 1, -alpha - 1, -alpha);
        if (result > alpha) {
            // Failed high
            if (result >= beta) {
                alpha = beta; // This will break the loop before the next iteration
            }
            else {
                alpha = -alphabeta(white, black, depth - 1, -beta, -result);
            }
            move = index;
        }
    }

    return (alpha * 256) | move;
}

int endgameAlphabeta(u64 black, u64 white, u64 lm, int alpha, int beta) {
    // Save initial board state
    u64 originalBlack = black;
    u64 originalWhite = white;

    // Factor to multiply return value by (1 or -1)
    int factor = 1;

    // Deal with no legal moves possibility
    if (lm == 0) {
        lm = findLegalMoves(white, black);
        if (lm == 0) {
            return DD(black, white);
        }

        // Swap sides and set factor
        u64 temp = originalBlack;
        originalBlack = originalWhite;
        originalWhite = temp;
        factor = -1;
        int tempInt = beta;
        beta = -alpha;
        alpha = -tempInt;
    }

    // Save original alpha and beta
    int originalAlpha = alpha;
    int originalBeta = beta;

    // Calculate total count for usage later
    int totalCount = TC(originalBlack, originalWhite);

    // Check endgame transposition table
    #if COUNT_COLLISIONS
    int canCollide = 1; // To count collisions later
    #endif
    u32 hash;
    if (totalCount < STOP_HASHING_TC) {
        hash = boardHash(originalBlack, originalWhite);
        if (endgameTT[hash].black == originalBlack && endgameTT[hash].white == originalWhite) {
            #if COUNT_COLLISIONS
            canCollide = 0;
            ttHits++;
            #endif
            u8 ee = EXTRACT_ENDGAME_TT_EVAL(endgameTT[hash].data);
            if (ee < 2) {
                return ee;
            }
            else if (ee == WHITE_WIN) {
                return -1;
            }
            else if (ee == NOT_BLACK_WIN) {
                if (alpha == 0) {
                    return 0; // Same window as original search
                }
                else beta = 0; // We know there is no black win, so we can bring beta down
            }
            else if (ee == NOT_WHITE_WIN) {
                if (beta == 0) {
                    return 0;
                }
                else alpha = 0;
            }
        }
    }

    // Declare result variable (to store results of endgame searches, and later final result)
    int result;

    // Total number of legal moves
    u8 numLegalMoves = PC(lm);

    // The legal moves will be ordered so that moves giving the opponent
    // less mobility appear first.

    // Array for moves, boards, and the number of legal moves each gives
    // Each "entry" is 2 * 8 (black and white boards) + 8 (score) = 3 bytes
    u64 arr[numLegalMoves * 3];

    // Index to keep track of place in arrays
    u8 moveIndex = 0;

    // Try alphabeta search suggestion
    if (totalCount < STOP_USING_EVAL - ENDGAME_AB_DEPTH) {
        int abResult = alphabetaMove(originalBlack, originalWhite, ENDGAME_AB_DEPTH, MIN_EVAL, MAX_EVAL);
        u8 move = EXTRACT_MOVE(abResult);
        black = doMove(originalBlack, originalWhite, move);
        white = originalWhite & ~black;
        if (totalCount == 61) {
            result = -endgameAlphabeta62(white, black, findLegalMoves(white, black), -beta, -alpha);
        }
        else {
            result = -endgameAlphabeta(white, black, findLegalMoves(white, black), -beta, -alpha);
        }
        alpha = (result > alpha) ? result : alpha;

        // Check if this move is good enough to return immediately
        if (alpha >= beta) goto save_to_hash;

        // Remove move from lm
        lm ^= BIT(move);
        numLegalMoves--;
    }

    // Loop through legal moves
    while (lm) {
        // Extract move
        u8 square = CLZ(lm);
        lm ^= BIT(square);

        // Make move
        black = doMove(originalBlack, originalWhite, square);
        white = originalWhite & ~black;

        // Store board, move, and number of opponent's legal moves
        arr[3 * moveIndex] = black;
        arr[3 * moveIndex + 1] = white;
        arr[3 * moveIndex + 2] = findLegalMoves(white, black);

        // Update moveIndex
        moveIndex++;
    }

    // Sort
    qsort(arr, numLegalMoves, 3 * sizeof(u64), ffComp);

    // Main alphabeta algorithm
    for (size_t i = 0; alpha < beta && i < numLegalMoves; i++) {
        // Recursive call and update alpha
        int result;
        if (totalCount == 61) {
            result = -endgameAlphabeta62(arr[3 * i + 1], arr[3 * i], arr[3 * i + 2], -beta, -alpha);
        }
        else {
            result = -endgameAlphabeta(arr[3 * i + 1], arr[3 * i], arr[3 * i + 2], -beta, -alpha);
        }
        alpha = (result > alpha) ? result : alpha;
    }

    save_to_hash:
    result = factor * alpha;

    // Save results to hash
    if (totalCount < STOP_HASHING_TC) {
        #if 0
        if (endgameTT[hash].black 
                                  #if COUNT_COLLISIONS
                                  && canCollide
                                  #endif
                                  ) {
            #if COUNT_COLLISIONS
            collisions[hash]++;
            #endif
            // Replacement scheme is always replace
        }
        #endif
        endgameTT[hash].black = originalBlack;
        endgameTT[hash].white = originalWhite;
        if (result <= -1) {
            endgameTT[hash].data = WHITE_WIN;
        }
        else if (result >= 1) {
            endgameTT[hash].data = BLACK_WIN;
        }
        else if (originalAlpha == -1 && originalBeta == 1) {
            endgameTT[hash].data = DRAW;
        }
        else if (originalAlpha == -1) {
            endgameTT[hash].data = NOT_WHITE_WIN;
        }
        else if (originalAlpha == 0) {
            endgameTT[hash].data = NOT_BLACK_WIN;
        }
    }

    return result;
}

int endgameAlphabeta63(u64 black, u64 white) {
    // Originals
    u64 originalBlack = black;

    // Make move
    u8 square = CLZ(~(black | white));
    black = doMove(black, white, square);
    white &= ~black;

    // Was move legal?
    if (black == originalBlack) {
        // Try white
        white = doMove(white, black, square);
        black &= ~white;
    }

    // Compute result
    return DD(black, white);
}

int endgameAlphabeta62(u64 black, u64 white, u64 lm, int alpha, int beta) {
    int factor = 1;
    if (lm == 0) {
        lm = findLegalMoves(white, black);
        if (lm == 0) {
            return DD(black, white);
        }

        // Swap sides and set factor
        u64 temp = black;
        black = white;
        white = temp;
        factor = -1;
        int tempInt = beta;
        beta = -alpha;
        alpha = -tempInt;
    }

    // Make move
    u8 square63 = CLZ(lm);
    lm ^= BIT(square63);
    u64 black1 = doMove(black, white, square63);
    u64 white1 = white & ~black1;
    int best = -endgameAlphabeta63(white1, black1);
    if (best >= beta) return factor * beta;
    
    if (lm) {
        // Try other move
        square63 = CLZ(lm);
        u64 black2 = doMove(black, white, square63);
        u64 white2 = white & ~black2;
        int secondEval = -endgameAlphabeta63(white2, black2);
        if (secondEval > best) return factor * secondEval;
    }

    return factor * best;
}

int endgameAlphabetaMove(u64 black, u64 white, int alpha, int beta) {
    // Deal with no legal moves possibility
    u64 lm = findLegalMoves(black, white);
    if (lm == 0) {
        lm = findLegalMoves(white, black);
        if (lm == 0) {
            return (DD(black, white) * 256) | NULL_MOVE;
        }
        return (-endgameAlphabeta(white, black, lm, -beta, -alpha) * 256) | NULL_MOVE;
    }
    
    // Set default move
    u8 move = CLZ(lm);
    
    // Save initial board state
    u64 originalBlack = black;
    u64 originalWhite = white;

    // Calculate total count for usage later
    int totalCount = TC(originalBlack, originalWhite);

    // Declare result variable (to store results of endgame searches, and later final result)
    int result;

    // Try alphabeta search suggestion
    if (totalCount < STOP_USING_EVAL - ENDGAME_AB_DEPTH) {
        int abResult = alphabetaMove(originalBlack, originalWhite, ENDGAME_AB_DEPTH, MIN_EVAL, MAX_EVAL);
        move = EXTRACT_MOVE(abResult);
        black = doMove(originalBlack, originalWhite, move);
        white = originalWhite & ~black;
        result = -endgameAlphabeta(white, black, findLegalMoves(white, black), -beta, -alpha);
        alpha = (result > alpha) ? result : alpha;

        // Check if this move is good enough to return immediately
        if (alpha >= beta) return (256 * beta) | move;
        
        // Remove tried move from lm
        lm ^= BIT(move);
    }

    // Total number of legal moves
    u8 numLegalMoves = PC(lm);

    // The legal moves will be ordered so that moves giving the opponent
    // less mobility appear first.

    // Array for moves, boards, and the number of legal moves each gives
    // Each "entry" is 2 * 8 (black and white boards) + 8 (score) = 3 bytes
    u64 arr[numLegalMoves * 3];

    // Index to keep track of place in arrays
    u8 moveIndex = 0;

    // Loop through legal moves
    while (lm) {
        // Extract move
        u8 square = CLZ(lm);
        lm ^= BIT(square);

        // Make move
        black = doMove(originalBlack, originalWhite, square);
        white = originalWhite & ~black;

        // Store board, move, and number of opponent's legal moves
        arr[3 * moveIndex] = black;
        arr[3 * moveIndex + 1] = white;
        arr[3 * moveIndex + 2] = ((u64) square << 56) | PC(findLegalMoves(white, black));

        // Update moveIndex
        moveIndex++;
    }

    // Sort
    qsort(arr, numLegalMoves, 3 * sizeof(u64), ffComp);

    // Main alphabeta algorithm
    for (size_t i = 0; alpha < beta && i < numLegalMoves; i++) {
        // Recursive call and update alpha
        int result = -endgameAlphabeta(arr[3 * i + 1], arr[3 * i], findLegalMoves(arr[3 * i + 1], arr[3 * i]), -beta, -alpha);
        if (result > alpha) {
            alpha = result;
            move = arr[3 * i + 2] >> 56;
        }
    }

    return (alpha * 256) | move;
}
