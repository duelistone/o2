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
        u8 firstMove = alphabetaMove(black, white, depth - 1, alpha, beta) & 0xFF;

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

        // Recursive call and update alpha
        int result = -alphabeta(white, black, depth - 1, -beta, -alpha);
        if (result > alpha) alpha = result;
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
        move = alphabetaMove(black, white, depth - 1, alpha, beta) & 0xFF;

        // Try first move
        lm ^= BIT(move);
        black = doMove(originalBlack, originalWhite, move);
        white = originalWhite & ~black;

        // Recursive call and update alpha
        int result = -alphabeta(white, black, depth - 1, -beta, -alpha);
        if (result > alpha) alpha = result;
    }

    // Main alphabeta algorithm
    while (lm && (alpha < beta)) {
        // Extract next legal move and make the move
        u8 index = CLZ(lm);
        lm ^= BIT(index);
        black = doMove(originalBlack, originalWhite, index);
        white = originalWhite & ~black;

        // Recursive call and update alpha
        int result = -alphabeta(white, black, depth - 1, -beta, -alpha);
        if (result > alpha) {
            alpha = result;
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

    if (TC(black, white) == 63) {
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
        
    // Deal with no legal moves possibility
    u64 lm = findLegalMoves(black, white);
    if (lm == 0) {
        if (findLegalMoves(white, black) == 0) {
            return DD(black, white);
        }
        return -endgameAlphabeta(white, black, -beta, -alpha);
    }

    // The legal moves will be ordered so that moves giving the opponent
    // less mobility appear first.

    // Array for moves, boards, and the number of legal moves each gives
    // Assumes a maximum of 50 legal moves
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
        int result = -endgameAlphabeta(arr[3 * i + 1], arr[3 * i], -beta, -alpha);
        if (result > alpha) alpha = result;
    }

    return alpha;
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
