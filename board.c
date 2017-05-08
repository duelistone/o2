// board.c

#include "board.h"

// Find legal moves function
// Returns a bitboard of legal moves
// It will find the legal moves for the color of 
// the first argument (called black in the function)
u64 findLegalMoves(u64 black, u64 white) {
    u64 moves = 0;
    u64 w, t;

    // RIGHT
    w = white & RIGHT_FILTER;
    t = w & SHIFT_RIGHT(black);
    if (t) {
        t |= w & SHIFT_RIGHT(t);
        t |= w & SHIFT_RIGHT(t);
        t |= w & SHIFT_RIGHT(t);
        t |= w & SHIFT_RIGHT(t);
        t |= w & SHIFT_RIGHT(t);
        moves |= RIGHT_FILTER & SHIFT_RIGHT(t);
    }

    // LEFT
    w = white & LEFT_FILTER;
    t = w & SHIFT_LEFT(black);
    if (t) {
        t |= w & SHIFT_LEFT(t);
        t |= w & SHIFT_LEFT(t);
        t |= w & SHIFT_LEFT(t);
        t |= w & SHIFT_LEFT(t);
        t |= w & SHIFT_LEFT(t);
        moves |= LEFT_FILTER & SHIFT_LEFT(t);
    }

    // DOWN
    w = white & DOWN_FILTER;
    t = w & SHIFT_DOWN(black);
    if (t) {
        t |= w & SHIFT_DOWN(t);
        t |= w & SHIFT_DOWN(t);
        t |= w & SHIFT_DOWN(t);
        t |= w & SHIFT_DOWN(t);
        t |= w & SHIFT_DOWN(t);
        moves |= DOWN_FILTER & SHIFT_DOWN(t);
    }

    // UP
    w = white & UP_FILTER;
    t = w & SHIFT_UP(black);
    if (t) {
        t |= w & SHIFT_UP(t);
        t |= w & SHIFT_UP(t);
        t |= w & SHIFT_UP(t);
        t |= w & SHIFT_UP(t);
        t |= w & SHIFT_UP(t);
        moves |= UP_FILTER & SHIFT_UP(t);
    }

    // DOWN_RIGHT
    w = white & DOWN_RIGHT_FILTER;
    t = w & SHIFT_DOWN_RIGHT(black);
    if (t) {
        t |= w & SHIFT_DOWN_RIGHT(t);
        t |= w & SHIFT_DOWN_RIGHT(t);
        t |= w & SHIFT_DOWN_RIGHT(t);
        t |= w & SHIFT_DOWN_RIGHT(t);
        t |= w & SHIFT_DOWN_RIGHT(t);
        moves |= DOWN_RIGHT_FILTER & SHIFT_DOWN_RIGHT(t);
    }

    // UP_RIGHT
    w = white & UP_RIGHT_FILTER;
    t = w & SHIFT_UP_RIGHT(black);
    if (t) {
        t |= w & SHIFT_UP_RIGHT(t);
        t |= w & SHIFT_UP_RIGHT(t);
        t |= w & SHIFT_UP_RIGHT(t);
        t |= w & SHIFT_UP_RIGHT(t);
        t |= w & SHIFT_UP_RIGHT(t);
        moves |= UP_RIGHT_FILTER & SHIFT_UP_RIGHT(t);
    }

    // DOWN_LEFT
    w = white & DOWN_LEFT_FILTER;
    t = w & SHIFT_DOWN_LEFT(black);
    if (t) {
        t |= w & SHIFT_DOWN_LEFT(t);
        t |= w & SHIFT_DOWN_LEFT(t);
        t |= w & SHIFT_DOWN_LEFT(t);
        t |= w & SHIFT_DOWN_LEFT(t);
        t |= w & SHIFT_DOWN_LEFT(t);
        moves |= DOWN_LEFT_FILTER & SHIFT_DOWN_LEFT(t);
    }

    // UP_LEFT
    w = white & UP_LEFT_FILTER;
    t = w & SHIFT_UP_LEFT(black);
    if (t) {
        t |= w & SHIFT_UP_LEFT(t);
        t |= w & SHIFT_UP_LEFT(t);
        t |= w & SHIFT_UP_LEFT(t);
        t |= w & SHIFT_UP_LEFT(t);
        t |= w & SHIFT_UP_LEFT(t);
        moves |= UP_LEFT_FILTER & SHIFT_UP_LEFT(t);
    }

    return moves & ~(white | black);
}

// Makes a move for the color of
// the first argument (called black in the function)
u64 doMove(u64 black, u64 white, u8 square) {
    u64 bi = BIT(square);
    u64 newblack = black;
    u64 filtered, n;
    
    #define DMB(x, filter, shifter, shift_amount) n = bi; filtered = white & filter; n |= filtered & (n shifter shift_amount); if (n - bi) {for (size_t i = 1; i < (x); i++) n |= filtered & (n shifter shift_amount); C_OR(black & filter & (n shifter shift_amount), newblack, n);}
    #define RIGHT(x) DMB(x, RIGHT_FILTER, >>, 1)
    #define DOWN(x) DMB(x, DOWN_FILTER, >>, 8)
    #define DOWN_RIGHT(x) DMB(x, DOWN_RIGHT_FILTER, >>, 9)
    #define DOWN_LEFT(x) DMB(x, DOWN_LEFT_FILTER, >>, 7)
    #define UP(x) DMB(x, UP_FILTER, <<, 8)
    #define UP_LEFT(x) DMB(x, UP_LEFT_FILTER, <<, 9)
    #define UP_RIGHT(x) DMB(x, UP_RIGHT_FILTER, <<, 7)
    #define LEFT(x) DMB(x, LEFT_FILTER, << , 1)

    switch (square) {
        case 0:
            RIGHT(6)
            DOWN(6)
            DOWN_RIGHT(6)
            return newblack;
        case 1:
            RIGHT(5)
            DOWN(6)
            DOWN_RIGHT(5)
            return newblack;
        case 2:
            RIGHT(4)
            DOWN(6)
            DOWN_RIGHT(4)
            DOWN_LEFT(1)
            LEFT(1)
            return newblack;
        case 3:
            RIGHT(3)
            DOWN(6)
            DOWN_RIGHT(3)
            DOWN_LEFT(2)
            LEFT(2)
            return newblack;
        case 4:
            RIGHT(2)
            DOWN(6)
            DOWN_RIGHT(2)
            DOWN_LEFT(3)
            LEFT(3)
            return newblack;
        case 5:
            RIGHT(1)
            DOWN(6)
            DOWN_RIGHT(1)
            DOWN_LEFT(4)
            LEFT(4)
            return newblack;
        case 6:
            DOWN(6)
            DOWN_LEFT(5)
            LEFT(5)
            return newblack;
        case 7:
            DOWN(6)
            DOWN_LEFT(6)
            LEFT(6)
            return newblack;
        case 8:
            RIGHT(6)
            DOWN(5)
            DOWN_RIGHT(5)
            return newblack;
        case 9:
            RIGHT(5)
            DOWN(5)
            DOWN_RIGHT(5)
            return newblack;
        case 10:
            RIGHT(4)
            DOWN(5)
            DOWN_RIGHT(4)
            DOWN_LEFT(1)
            LEFT(1)
            return newblack;
        case 11:
            RIGHT(3)
            DOWN(5)
            DOWN_RIGHT(3)
            DOWN_LEFT(2)
            LEFT(2)
            return newblack;
        case 12:
            RIGHT(2)
            DOWN(5)
            DOWN_RIGHT(2)
            DOWN_LEFT(3)
            LEFT(3)
            return newblack;
        case 13:
            RIGHT(1)
            DOWN(5)
            DOWN_RIGHT(1)
            DOWN_LEFT(4)
            LEFT(4)
            return newblack;
        case 14:
            DOWN(5)
            DOWN_LEFT(5)
            LEFT(5)
            return newblack;
        case 15:
            DOWN(5)
            DOWN_LEFT(5)
            LEFT(6)
            return newblack;
        case 16:
            RIGHT(6)
            DOWN(4)
            DOWN_RIGHT(4)
            UP(1)
            UP_RIGHT(1)
            return newblack;
        case 17:
            RIGHT(5)
            DOWN(4)
            DOWN_RIGHT(4)
            UP(1)
            UP_RIGHT(1)
            return newblack;
        case 18:
            RIGHT(4)
            DOWN(4)
            DOWN_RIGHT(4)
            DOWN_LEFT(1)
            LEFT(1)
            UP(1)
            UP_LEFT(1)
            UP_RIGHT(1)
            return newblack;
        case 19:
            RIGHT(3)
            DOWN(4)
            DOWN_RIGHT(3)
            DOWN_LEFT(2)
            LEFT(2)
            UP(1)
            UP_LEFT(1)
            UP_RIGHT(1)
            return newblack;
        case 20:
            RIGHT(2)
            DOWN(4)
            DOWN_RIGHT(2)
            DOWN_LEFT(3)
            LEFT(3)
            UP(1)
            UP_LEFT(1)
            UP_RIGHT(1)
            return newblack;
        case 21:
            RIGHT(1)
            DOWN(4)
            DOWN_RIGHT(1)
            DOWN_LEFT(4)
            LEFT(4)
            UP(1)
            UP_LEFT(1)
            UP_RIGHT(1)
            return newblack;
        case 22:
            DOWN(4)
            DOWN_LEFT(4)
            LEFT(5)
            UP(1)
            UP_LEFT(1)
            return newblack;
        case 23:
            DOWN(4)
            DOWN_LEFT(4)
            LEFT(6)
            UP(1)
            UP_LEFT(1)
            return newblack;
        case 24:
            RIGHT(6)
            DOWN(3)
            DOWN_RIGHT(3)
            UP(2)
            UP_RIGHT(2)
            return newblack;
        case 25:
            RIGHT(5)
            DOWN(3)
            DOWN_RIGHT(3)
            UP(2)
            UP_RIGHT(2)
            return newblack;
        case 26:
            RIGHT(4)
            DOWN(3)
            DOWN_RIGHT(3)
            DOWN_LEFT(1)
            LEFT(1)
            UP(2)
            UP_LEFT(1)
            UP_RIGHT(2)
            return newblack;
        case 27:
            RIGHT(3)
            DOWN(3)
            DOWN_RIGHT(3)
            DOWN_LEFT(2)
            LEFT(2)
            UP(2)
            UP_LEFT(2)
            UP_RIGHT(2)
            return newblack;
        case 28:
            RIGHT(2)
            DOWN(3)
            DOWN_RIGHT(2)
            DOWN_LEFT(3)
            LEFT(3)
            UP(2)
            UP_LEFT(2)
            UP_RIGHT(2)
            return newblack;
        case 29:
            RIGHT(1)
            DOWN(3)
            DOWN_RIGHT(1)
            DOWN_LEFT(3)
            LEFT(4)
            UP(2)
            UP_LEFT(2)
            UP_RIGHT(1)
            return newblack;
        case 30:
            DOWN(3)
            DOWN_LEFT(3)
            LEFT(5)
            UP(2)
            UP_LEFT(2)
            return newblack;
        case 31:
            DOWN(3)
            DOWN_LEFT(3)
            LEFT(6)
            UP(2)
            UP_LEFT(2)
            return newblack;
        case 32:
            RIGHT(6)
            UP(3)
            UP_RIGHT(3)
            DOWN(2)
            DOWN_RIGHT(2)
            return newblack;
        case 33:
            RIGHT(5)
            UP(3)
            UP_RIGHT(3)
            DOWN(2)
            DOWN_RIGHT(2)
            return newblack;
        case 34:
            RIGHT(4)
            UP(3)
            UP_RIGHT(3)
            UP_LEFT(1)
            LEFT(1)
            DOWN(2)
            DOWN_LEFT(1)
            DOWN_RIGHT(2)
            return newblack;
        case 35:
            RIGHT(3)
            UP(3)
            UP_RIGHT(3)
            UP_LEFT(2)
            LEFT(2)
            DOWN(2)
            DOWN_LEFT(2)
            DOWN_RIGHT(2)
            return newblack;
        case 36:
            RIGHT(2)
            UP(3)
            UP_RIGHT(2)
            UP_LEFT(3)
            LEFT(3)
            DOWN(2)
            DOWN_LEFT(2)
            DOWN_RIGHT(2)
            return newblack;
        case 37:
            RIGHT(1)
            UP(3)
            UP_RIGHT(1)
            UP_LEFT(3)
            LEFT(4)
            DOWN(2)
            DOWN_LEFT(2)
            DOWN_RIGHT(1)
            return newblack;
        case 38:
            UP(3)
            UP_LEFT(3)
            LEFT(5)
            DOWN(2)
            DOWN_LEFT(2)
            return newblack;
        case 39:
            UP(3)
            UP_LEFT(3)
            LEFT(6)
            DOWN(2)
            DOWN_LEFT(2)
            return newblack;
        case 40:
            RIGHT(6)
            UP(4)
            UP_RIGHT(4)
            DOWN(1)
            DOWN_RIGHT(1)
            return newblack;
        case 41:
            RIGHT(5)
            UP(4)
            UP_RIGHT(4)
            DOWN(1)
            DOWN_RIGHT(1)
            return newblack;
        case 42:
            RIGHT(4)
            UP(4)
            UP_RIGHT(4)
            UP_LEFT(1)
            LEFT(1)
            DOWN(1)
            DOWN_LEFT(1)
            DOWN_RIGHT(1)
            return newblack;
        case 43:
            RIGHT(3)
            UP(4)
            UP_RIGHT(3)
            UP_LEFT(2)
            LEFT(2)
            DOWN(1)
            DOWN_LEFT(1)
            DOWN_RIGHT(1)
            return newblack;
        case 44:
            RIGHT(2)
            UP(4)
            UP_RIGHT(2)
            UP_LEFT(4)
            LEFT(3)
            DOWN(1)
            DOWN_LEFT(1)
            DOWN_RIGHT(1)
            return newblack;
        case 45:
            RIGHT(1)
            UP(4)
            UP_RIGHT(1)
            UP_LEFT(4)
            LEFT(4)
            DOWN(1)
            DOWN_LEFT(1)
            DOWN_RIGHT(1)
            return newblack;
        case 46:
            UP(4)
            UP_LEFT(4)
            LEFT(5)
            DOWN(1)
            DOWN_LEFT(1)
            return newblack;
        case 47:
            UP(4)
            UP_LEFT(4)
            LEFT(6)
            DOWN(1)
            DOWN_LEFT(1)
            return newblack;
        case 48:
            RIGHT(6)
            UP(5)
            UP_RIGHT(5)
            return newblack;
        case 49:
            RIGHT(5)
            UP(5)
            UP_RIGHT(5)
            return newblack;
        case 50:
            RIGHT(4)
            UP(5)
            UP_RIGHT(4)
            UP_LEFT(1)
            LEFT(1)
            return newblack;
        case 51:
            RIGHT(3)
            UP(5)
            UP_RIGHT(3)
            UP_LEFT(2)
            LEFT(2)
            return newblack;
        case 52:
            RIGHT(2)
            UP(5)
            UP_RIGHT(2)
            UP_LEFT(3)
            LEFT(3)
            return newblack;
        case 53:
            RIGHT(1)
            UP(5)
            UP_RIGHT(1)
            UP_LEFT(4)
            LEFT(4)
            return newblack;
        case 54:
            UP(5)
            UP_LEFT(5)
            LEFT(5)
            return newblack;
        case 55:
            UP(5)
            UP_LEFT(5)
            LEFT(6)
            return newblack;
        case 56:
            RIGHT(6)
            UP(6)
            UP_RIGHT(6)
            return newblack;
        case 57:
            RIGHT(5)
            UP(6)
            UP_RIGHT(5)
            return newblack;
        case 58:
            RIGHT(4)
            UP(6)
            UP_RIGHT(4)
            UP_LEFT(1)
            LEFT(1)
            return newblack;
        case 59:
            RIGHT(3)
            UP(6)
            UP_RIGHT(3)
            UP_LEFT(2)
            LEFT(2)
            return newblack;
        case 60:
            RIGHT(2)
            UP(6)
            UP_RIGHT(2)
            UP_LEFT(3)
            LEFT(3)
            return newblack;
        case 61:
            RIGHT(1)
            UP(6)
            UP_RIGHT(1)
            UP_LEFT(4)
            LEFT(4)
            return newblack;
        case 62:
            UP(6)
            UP_LEFT(5)
            LEFT(5)
            return newblack;
        case 63:
            UP(6)
            UP_LEFT(6)
            LEFT(6)
            return newblack;
    }

    return 0; // To signal an error
}

u64 frontier(u64 black, u64 white) {
    u64 empty = ~(black | white);
    u64 frontierBlack = SHIFT_RIGHT(empty) & ~(RIGHT_FILTER | LEFT_FILTER) & black;
    frontierBlack |= SHIFT_LEFT(empty) & ~(LEFT_FILTER | RIGHT_FILTER) & black;
    frontierBlack |= SHIFT_UP(empty) & ~(UP_FILTER | DOWN_FILTER) & black;
    frontierBlack |= SHIFT_DOWN(empty) & ~(DOWN_FILTER | UP_FILTER) & black;
    frontierBlack |= SHIFT_UP_RIGHT(empty) & ~EDGES & black;
    frontierBlack |= SHIFT_UP_LEFT(empty) & ~EDGES & black;
    frontierBlack |= SHIFT_DOWN_RIGHT(empty) & ~EDGES & black;
    frontierBlack |= SHIFT_DOWN_LEFT(empty) & ~EDGES & black;
    return frontierBlack;
}

// Evaluate position to int
// Assumes the first argument is to move
// Also assumes a wipeout has not occurred.
u32 eval(u64 black, u64 white) {
    // Frontier
    // Idea: double frontier (2 away and unshielded)?
    double blackFrontierRatio = PC(frontier(black, white)) / (double) PC(black);
    double whiteFrontierRatio = PC(frontier(white, black)) / (double) PC(white);

    double ee = 0.1 * (whiteFrontierRatio - blackFrontierRatio) + 0.9 * (PC(black & CORNERS) - PC(white & CORNERS)) / 4;

    return 1000 * ee;
}

// Print board
void printBoard(u64 black, u64 white) {
    char s[200];
    u8 nextIndex = 0;
    printf("+--------+\n");
    for (u8 i = 0; i < 8; i++) {
        s[nextIndex] = '|';
        nextIndex++;
        for (u8 j = 0; j < 8; j++) {
            if (GET(black, j, i)) s[nextIndex] = 'X';
            else if (GET(white, j, i)) s[nextIndex] = 'O';
            else s[nextIndex] = ' ';
            nextIndex++;
        }
        s[nextIndex] = '|';
        nextIndex++;
        s[nextIndex] = '\n';
        nextIndex++;
    }
    s[nextIndex] = '\0';

    printf("%s", s);
    printf("+--------+\n");
}
void printBoard2(u64 black, u64 white) {
    char s[200];
    u8 nextIndex = 0;
    fprintf(stderr, "+--------+\n");
    for (u8 i = 0; i < 8; i++) {
        s[nextIndex] = '|';
        nextIndex++;
        for (u8 j = 0; j < 8; j++) {
            if (GET(black, j, i)) s[nextIndex] = 'X';
            else if (GET(white, j, i)) s[nextIndex] = 'O';
            else s[nextIndex] = ' ';
            nextIndex++;
        }
        s[nextIndex] = '|';
        nextIndex++;
        s[nextIndex] = '\n';
        nextIndex++;
    }
    s[nextIndex] = '\0';

    fprintf(stderr, "%s", s);
    fprintf(stderr, "+--------+\n");
}
