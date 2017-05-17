// search.c

#include "search.h"

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

    // Return static evaluation at depth 0 or less (yes, less is possible due to lm == 0 case)
    if (depth <= 0) {
        return eval(black, white);
    }
    
    // Save initial board state
    u64 originalBlack = black;
    u64 originalWhite = white;

    // Iterative deepening
    if (depth > 1) {
        //puts("Depth not 1");
        u8 firstMove = EXTRACT_MOVE(alphabetaMove(black, white, depth - 1, alpha, beta));

        // Try first move
        lm ^= BIT(firstMove);
        black = doMove(originalBlack, originalWhite, firstMove);
        white = originalWhite & ~black;

        // Recursive call and update alpha
        int result = -alphabeta(white, black, depth - 1, -beta, -alpha);
        if (result > alpha) alpha = result;
    }

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
            if (result >= beta) return beta; // Avoid calling alphabeta with alpha >= beta
            alpha = -alphabeta(white, black, depth - 1, -beta, -alpha);
        }
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
        move = EXTRACT_MOVE(alphabetaMove(black, white, depth - 1, alpha, beta));

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
                alpha = -alphabeta(white, black, depth - 1, -beta, MAX_EVAL);
            }
            //printf("changing move to %d depth %d\n", index, depth);
            move = index;
        }
    }

    return (alpha * 256) | move;
}

// Comparison function for fastest first heuristic
// Look at the endgame alphabeta function to see the array
// this is working on.
int ffComp(const void *a, const void *b) {
    // Want to compare the (unsigned) numbers formed by last of 
    // the three u64's.
    return (int) *((u64 *) a + 2) - (int) *((u64 *) b + 2);
}

int endgameAlphabeta(u64 black, u64 white, int alpha, int beta) {
    // Save initial board state
    u64 originalBlack = black;
    u64 originalWhite = white;

    // Factor to multiply return value by (1 or -1)
    int factor = 1;

    // Deal with no legal moves possibility
    u64 lm = findLegalMoves(black, white);
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
    }

    // The legal moves will be ordered so that moves giving the opponent
    // less mobility appear first.

    // Array for moves, boards, and the number of legal moves each gives
    // Each "entry" is 2 * 8 (black and white boards) + 8 (score) = 3 bytes
    u64 arr[(64 - ENDGAME_START) * 3];

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
        int result;
        if (TC(originalBlack, originalWhite) == 61) {
            result = -endgameAlphabeta62(arr[3 * i + 1], arr[3 * i], -beta, -alpha);
        }
        else {
            result = -endgameAlphabeta(arr[3 * i + 1], arr[3 * i], -beta, -alpha);
        }
        alpha = (result > alpha) ? result : alpha;
    }

    return factor * alpha;
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

int endgameAlphabeta62(u64 black, u64 white, int alpha, int beta) {
    int factor = 1;
    u64 lm = findLegalMoves(black, white);
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
    }

    // Make move
    u8 square63 = CLZ(lm);
    lm ^= BIT(square63);
    u64 black1 = doMove(black, white, square63);
    u64 white1 = white & ~black1;
    int best = -endgameAlphabeta63(white1, black1);
    if (best >= beta) return factor * beta;
    
    if (lm != 0) {
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
        return (-endgameAlphabeta(white, black, -beta, -alpha) * 256) | NULL_MOVE;
    }

    // Set default move
    u8 move = CLZ(lm);
    
    // Save initial board state
    u64 originalBlack = black;
    u64 originalWhite = white;

    // Main alphabeta algorithm
    while (lm && alpha < beta) {
        // Extract next legal move and make the move
        u8 index = CLZ(lm);
        lm ^= BIT(index);
        black = doMove(originalBlack, originalWhite, index);
        white = originalWhite & ~black;

        // Recursive call and update alpha
        int result = -endgameAlphabeta(white, black, -beta, -alpha);
        if (result > alpha) {
            alpha = result;
            move = index;
        }
    }

    return (alpha * 256) | move;
}
