// board.c

#include "board.h"

// Nonnegative integer exponentiation function
// Based on Wikipedia's algorithm for "Exponentiation by Squaring"
double int_pow(double base, int exponent) {
    // Nonpositive case; negative exponents are treated as 0.
    if (exponent <= 0) return 1;

    // Variable to store factors produced in odd exponent cases
    double y = 1;

    while (exponent > 1) {
        if (exponent % 2 == 0) {
            // Even case, so we square the base here
            base *= base;
            exponent /= 2;
        }
        else {
            // Odd case, so we take extract one factor to get
            // back to the even case and then deal with that here
            y *= base;
            base *= base;
            exponent = (exponent - 1) / 2;
        }
    }

    return base * y;
}

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
    u64 frontierBlack = SHIFT_RIGHT(empty) & (RIGHT_FILTER & LEFT_FILTER) & black;
    frontierBlack |= SHIFT_LEFT(empty) & (LEFT_FILTER & RIGHT_FILTER) & black;
    frontierBlack |= SHIFT_UP(empty) & (UP_FILTER & DOWN_FILTER) & black;
    frontierBlack |= SHIFT_DOWN(empty) & (DOWN_FILTER & UP_FILTER) & black;
    frontierBlack |= SHIFT_UP_RIGHT(empty) & ~EDGES & black;
    frontierBlack |= SHIFT_UP_LEFT(empty) & ~EDGES & black;
    frontierBlack |= SHIFT_DOWN_RIGHT(empty) & ~EDGES & black;
    frontierBlack |= SHIFT_DOWN_LEFT(empty) & ~EDGES & black;
    return frontierBlack;
}

double frontierScore(u64 black, u64 white, u64 poison) {
    u64 taken = black | white;
    u64 empty = ~taken;
    // Calculate weighted frontier
    int result = PC(SHIFT_RIGHT(empty & ~poison) & (RIGHT_FILTER & LEFT_FILTER) & black);
    result += PC(SHIFT_LEFT(empty & ~poison) & (LEFT_FILTER & RIGHT_FILTER) & black);
    result += PC(SHIFT_UP(empty & ~poison) & (UP_FILTER & DOWN_FILTER) & black);
    result += PC(SHIFT_DOWN(empty & ~poison) & (DOWN_FILTER & UP_FILTER) & black);
    result += PC(SHIFT_UP_RIGHT(empty & ~poison) & ~EDGES & black);
    result += PC(SHIFT_UP_LEFT(empty & ~poison) & ~EDGES & black);
    result += PC(SHIFT_DOWN_RIGHT(empty & ~poison) & ~EDGES & black);
    result += PC(SHIFT_DOWN_LEFT(empty & ~poison) & ~EDGES & black);
    u64 fro = frontier(black, white);
    u64 internal = black & ~fro;
    return (result - 4 * PC(internal & ~EDGES) - 2 * PC(internal & EDGES)) / (3 * PC(taken)) + PC(fro) / 8.0;
}

// Fills up an array with regions of board
// Returns the number of regions
u8 findRegions(u64 empty, u64 *regions) {
    u8 counter = 0;
    while (empty) {
        u64 region = BIT(CLZ(empty));
        u64 oldRegion;

        shifting_start:
        oldRegion = region;
        region |= SHIFT_RIGHT(region) & empty & RIGHT_FILTER;
        if (oldRegion != region) goto shifting_start;
        region |= SHIFT_DOWN(region) & empty & DOWN_FILTER;
        if (oldRegion != region) goto shifting_start;
        region |= SHIFT_DOWN_RIGHT(region) & empty & DOWN_RIGHT_FILTER;
        if (oldRegion != region) goto shifting_start;
        region |= SHIFT_LEFT(region) & empty & LEFT_FILTER;
        if (oldRegion != region) goto shifting_start;
        region |= SHIFT_DOWN_LEFT(region) & empty & DOWN_LEFT_FILTER;
        if (oldRegion != region) goto shifting_start;
        region |= SHIFT_UP_RIGHT(region) & empty & UP_RIGHT_FILTER;
        if (oldRegion != region) goto shifting_start;
        region |= SHIFT_UP_LEFT(region) & empty & UP_LEFT_FILTER;
        if (oldRegion != region) goto shifting_start;
        region |= SHIFT_UP(region) & empty & UP_FILTER;
        if (oldRegion != region) goto shifting_start;

        // Update regions, empty squares, and counter
        regions[counter] = region;
        empty ^= region;
        counter++;
    }
    return counter;
}

// Find margin of a region (the empty squares around it
// that can be an influence on the frontier)
u64 findMargin(u64 region) {
    u64 empty = ~region;
    u64 margin = empty & SHIFT_LEFT(region) & LEFT_FILTER & SHIFT_LEFT(LEFT_FILTER);
    margin |= empty & SHIFT_RIGHT(region) & RIGHT_FILTER & SHIFT_RIGHT(RIGHT_FILTER);
    margin |= empty & SHIFT_UP(region) & UP_FILTER & SHIFT_UP(UP_FILTER);
    margin |= empty & SHIFT_DOWN(region) & DOWN_FILTER & SHIFT_DOWN(DOWN_FILTER);
    margin |= empty & SHIFT_UP_RIGHT(region) & UP_RIGHT_FILTER & SHIFT_UP_RIGHT(UP_RIGHT_FILTER);
    margin |= empty & SHIFT_UP_LEFT(region) & UP_LEFT_FILTER & SHIFT_UP_LEFT(UP_LEFT_FILTER);
    margin |= empty & SHIFT_DOWN_RIGHT(region) & DOWN_RIGHT_FILTER & SHIFT_DOWN_RIGHT(DOWN_RIGHT_FILTER);
    margin |= empty & SHIFT_DOWN_LEFT(region) & DOWN_LEFT_FILTER & SHIFT_DOWN_LEFT(DOWN_LEFT_FILTER);

    return margin;
}

// Evaluate position to int
// Assumes the first argument is to move
// Also assumes a wipeout has not occurred, and that there is a legal move
int eval(u64 black, u64 white) {
    // To avoid worrying about keeping this code the same three times in this function 
    // and the next two, it will be included in (in an admittedly odd usage of #include).
    #include "eval_code.c"
    return ee / (1 + eeSumAbs / 400);
}

// Print eval information for debugging
void printEval(u64 black, u64 white) {
    #include "eval_code.c"
    printf("frontier: %f\n", frontierWeight * eeF);
    printf("mobility: %f\n", mobilityWeight * eeM);
    printf("corners: %f\n", cornerWeight * eeC);
    printf("regional mobility %f\n", rmWeight * eeRM);
    printf("ee %f\n", ee);
    printf("factor %f\n", 1 / (1 + eeSumAbs / 400));
    printf("Actual eval: %d\n", eval(black, white));
}

void printEval2(u64 black, u64 white) {
    #include "eval_code.c"
    fprintf(stderr, "frontier: %f\n", frontierWeight * eeF);
    fprintf(stderr, "mobility: %f\n", mobilityWeight * eeM);
    fprintf(stderr, "corners: %f\n", cornerWeight * eeC);
    fprintf(stderr, "regional mobility %f\n", rmWeight * eeRM);
    fprintf(stderr, "ee %f\n", ee);
    fprintf(stderr, "factor %f\n", 1 / (1 + eeSumAbs / 400));
    fprintf(stderr, "Actual eval: %d\n", eval(black, white));
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
