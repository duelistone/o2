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

double frontierScore(u64 black, u64 white) {
    u64 empty = ~(black | white);
    int result = PC(SHIFT_RIGHT(empty) & (RIGHT_FILTER & LEFT_FILTER) & black);
    result += PC(SHIFT_LEFT(empty) & (LEFT_FILTER & RIGHT_FILTER) & black);
    result += PC(SHIFT_UP(empty) & (UP_FILTER & DOWN_FILTER) & black);
    result += PC(SHIFT_DOWN(empty) & (DOWN_FILTER & UP_FILTER) & black);
    result += PC(SHIFT_UP_RIGHT(empty) & ~EDGES & black);
    result += PC(SHIFT_UP_LEFT(empty) & ~EDGES & black);
    result += PC(SHIFT_DOWN_RIGHT(empty) & ~EDGES & black);
    result += PC(SHIFT_DOWN_LEFT(empty) & ~EDGES & black);
    u64 fro = frontier(black, white);
    u64 internal = black & ~fro;
    return (result - 1.5 * PC(internal & ~EDGES) - 0.75 * PC(internal & EDGES)) / (5.0 * PC(black)) + PC(fro) / 10.0;
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
    u64 taken = black | white;
    u64 empty = ~taken;
    // Frontier
    // Idea: double frontier (2 away and unshielded)?
    // Old idea: simple frontier ratio
    //double blackFrontierRatio = PC(frontier(black, white)) / (double) PC(black);
    //double whiteFrontierRatio = PC(frontier(white, black)) / (double) PC(white);
    // New idea: Weighted frontiers
    double eeF = frontierScore(white, black) - frontierScore(black, white);

    // Corners
    double eeC = (PC(black & CORNERS) - PC(white & CORNERS));
    // Score for corner patterns
    #define SCP(x) ((black & (x)) == (x)) - ((white & (x)) == (x));
    eeC += SCP(A1 | B1);
    eeC += SCP(A1 | A2);
    eeC += SCP(H1 | G1);
    eeC += SCP(H1 | H2);
    eeC += SCP(A8 | B8);
    eeC += SCP(A8 | A7);
    eeC += SCP(H8 | G8);
    eeC += SCP(H8 | H7);
    eeC += SCP(A1 | B1 | C1);
    eeC += SCP(A1 | A2 | A3);
    eeC += SCP(H1 | G1 | F1);
    eeC += SCP(H1 | H2 | H3);
    eeC += SCP(A8 | B8 | C8);
    eeC += SCP(A8 | A7 | A6);
    eeC += SCP(H8 | G8 | F8);
    eeC += SCP(H8 | H7 | H6);

    // Poison squares (squares that give up corner, or risky x-squares)
    // This computation is finished in the C-squares section
    u64 poisonBlack = 0;
    u64 poisonWhite = 0;
    // X-square check
    #define XSQC(a, b) \
    if ((taken & ((a) | (b))) == 0) {poisonBlack |= (b); poisonWhite |= (b);}
    XSQC(A1, B2);
    XSQC(H1, G2);
    XSQC(A8, B7);
    XSQC(H8, G7);
    // Empty c-square check
    #define EMPTY_CSQ(a, b, c) \
    if ((taken & ((a) | (b))) == 0) {\
        if (white & (c)) poisonBlack |= (b);\
        else if (black & (c)) poisonWhite |= (b);\
    }
    EMPTY_CSQ(A1, B1, C1);
    EMPTY_CSQ(A1, A2, A3);
    EMPTY_CSQ(H1, H2, H3);
    EMPTY_CSQ(H1, G1, F1);
    EMPTY_CSQ(A8, B8, C8);
    EMPTY_CSQ(A8, A7, A6);
    EMPTY_CSQ(H8, H7, H6);
    EMPTY_CSQ(H8, G8, F8);

    // C squares
    // More isolated c-squares are worse
    // Macro for modifying c-square score based on corner a and adjacent c-square b
    #define CSQS(a, b, shifter) \
    if ((black & ((a) | (b))) == (b)) {\
        u64 c_region = b;\
        c_region |= shifter(c_region) & black;\
        c_region |= shifter(c_region) & black;\
        c_region |= shifter(c_region) & black;\
        c_region |= shifter(c_region) & black;\
        eeC -= 1.25 - PC(c_region) / 4.0;\
        c_region = shifter(c_region) & empty;\
        if (c_region && (shifter(c_region) & white)) poisonBlack |= c_region;\
    }\
    else if ((white & ((a) | (b))) == (b)) {\
        u64 c_region = (b);\
        c_region |= shifter(c_region) & white;\
        c_region |= shifter(c_region) & white;\
        c_region |= shifter(c_region) & white;\
        c_region |= shifter(c_region) & white;\
        eeC += 1.25 - PC(c_region) / 4.0;\
        c_region = shifter(c_region) & empty;\
        if (c_region && (shifter(c_region) & black)) poisonWhite |= c_region;\
    }
    CSQS(A1, B1, SHIFT_RIGHT);
    CSQS(A1, A2, SHIFT_DOWN);
    CSQS(H1, G1, SHIFT_LEFT);
    CSQS(H1, H2, SHIFT_DOWN);
    CSQS(A8, B8, SHIFT_RIGHT);
    CSQS(A8, A7, SHIFT_UP);
    CSQS(H8, G8, SHIFT_LEFT);
    CSQS(H8, H7, SHIFT_UP);

    // Scale down corner and c-square scores
    eeC /= (1 + PC(EDGES & taken)) / 2.4;
    
    // Mobility
    u64 lmBlack = findLegalMoves(black, white);
    u64 lmWhite = findLegalMoves(white, black);
    int lmNumBlack = PC(lmBlack);
    int lmNumWhite = PC(lmWhite);
    int diff = lmNumBlack - lmNumWhite - 1; // The -1 is a penalty for having to move
    int absDiff = (diff > 0) ? diff : -diff;
    double exponent = int_pow(TENTH_POWER_OF_FOUR, 35 - lmNumBlack - lmNumWhite);
    double eeM = 1 - int_pow(1 - 0.01 * absDiff, exponent); // No explanation for this formula yet, still testing
    eeM = (diff > 0) ? eeM : -eeM;

    // Regional mobility
    double eeRM = 0;
    u64 regions[15];
    u8 numRegions = findRegions(~(black | white), regions);
    for (int i = 0; i < numRegions; i++) {
        u64 region = regions[i];

        // Calculate legal move number for each region
        // Poison legal moves are treated as negative legal moves
        // Perhaps X-squares should only be subtracted once, since a sacrifice of a 
        // corner along an X-square is more common than a sacrifice along an edge.
        int lmNumBlack = PC(lmBlack & region);
        int lmNumWhite = PC(lmWhite & region);
        int total = lmNumBlack + lmNumWhite; // Total should not be affected by poison squares
        if (total == 0) continue;
        lmNumBlack -= 2 * PC(lmBlack & region & poisonBlack);
        lmNumWhite -= 2 * PC(lmWhite & region & poisonWhite);

        // Calculations (no explanations for these yet, but we use an exponential)
        int diff = lmNumBlack - lmNumWhite;
        int absDiff = (diff > 0) ? diff : -diff;
        double exponent = int_pow(TENTH_POWER_OF_FOUR, 35 - total);
        double score = 1 - int_pow(1 - 0.01 * absDiff, exponent); 
        score = (diff > 0) ? score : -score;
        if (PC(region & CORNERS) == 1) score *= 1.5;
        int factor = 2.1 - 0.1 * PC(region);
        factor = (factor > 1) ? factor : 1;
        score *= factor;
        eeRM += score;
    }
    eeRM /= numRegions;

    // Putting it all together
    double cornerWeight = 0.55;
    double remainingWeight = 1 - cornerWeight;
    double frontierWeight = 0.3 * remainingWeight;
    double mobilityWeight = 0.4 * remainingWeight;
    double rmWeight = 0.3 * remainingWeight;
    return 1024 * (eeF * frontierWeight + eeC * cornerWeight + eeM * mobilityWeight + eeRM * rmWeight);
}

// Print eval information for debugging
void printEval(u64 black, u64 white) {
    u64 taken = black | white;
    u64 empty = ~taken;
    // Frontier
    // Idea: double frontier (2 away and unshielded)?
    // Old idea: simple frontier ratio
    //double blackFrontierRatio = PC(frontier(black, white)) / (double) PC(black);
    //double whiteFrontierRatio = PC(frontier(white, black)) / (double) PC(white);
    // New idea: Weighted frontiers
    double eeF = frontierScore(white, black) - frontierScore(black, white);

    // Corners
    double eeC = (PC(black & CORNERS) - PC(white & CORNERS));
    // Score for corner patterns
    eeC += SCP(A1 | B1);
    eeC += SCP(A1 | A2);
    eeC += SCP(H1 | G1);
    eeC += SCP(H1 | H2);
    eeC += SCP(A8 | B8);
    eeC += SCP(A8 | A7);
    eeC += SCP(H8 | G8);
    eeC += SCP(H8 | H7);
    eeC += SCP(A1 | B1 | C1);
    eeC += SCP(A1 | A2 | A3);
    eeC += SCP(H1 | G1 | F1);
    eeC += SCP(H1 | H2 | H3);
    eeC += SCP(A8 | B8 | C8);
    eeC += SCP(A8 | A7 | A6);
    eeC += SCP(H8 | G8 | F8);
    eeC += SCP(H8 | H7 | H6);

    // Poison squares (squares that give up corner, or risky x-squares)
    // This computation is finished in the C-squares section
    u64 poisonBlack = 0;
    u64 poisonWhite = 0;
    // X-square check
    XSQC(A1, B2);
    XSQC(H1, G2);
    XSQC(A8, B7);
    XSQC(H8, G7);
    // Empty c-square check
    EMPTY_CSQ(A1, B1, C1);
    EMPTY_CSQ(A1, A2, A3);
    EMPTY_CSQ(H1, H2, H3);
    EMPTY_CSQ(H1, G1, F1);
    EMPTY_CSQ(A8, B8, C8);
    EMPTY_CSQ(A8, A7, A6);
    EMPTY_CSQ(H8, H7, H6);
    EMPTY_CSQ(H8, G8, F8);

    // C squares
    // More isolated c-squares are worse
    // Macro for modifying c-square score based on corner a and adjacent c-square b
    CSQS(A1, B1, SHIFT_RIGHT);
    CSQS(A1, A2, SHIFT_DOWN);
    CSQS(H1, G1, SHIFT_LEFT);
    CSQS(H1, H2, SHIFT_DOWN);
    CSQS(A8, B8, SHIFT_RIGHT);
    CSQS(A8, A7, SHIFT_UP);
    CSQS(H8, G8, SHIFT_LEFT);
    CSQS(H8, H7, SHIFT_UP);

    // Scale down corner and c-square scores
    eeC /= (1 + PC(EDGES & taken)) / 2.4;
    
    // Mobility
    u64 lmBlack = findLegalMoves(black, white);
    u64 lmWhite = findLegalMoves(white, black);
    int lmNumBlack = PC(lmBlack);
    int lmNumWhite = PC(lmWhite);
    int diff = lmNumBlack - lmNumWhite - 1; // The -1 is a penalty for having to move
    int absDiff = (diff > 0) ? diff : -diff;
    double exponent = int_pow(TENTH_POWER_OF_FOUR, 35 - lmNumBlack - lmNumWhite);
    double eeM = 1 - int_pow(1 - 0.01 * absDiff, exponent); // No explanation for this formula yet, still testing
    eeM = (diff > 0) ? eeM : -eeM;

    // Regional mobility
    double eeRM = 0;
    u64 regions[15];
    u8 numRegions = findRegions(~(black | white), regions);
    for (int i = 0; i < numRegions; i++) {
        u64 region = regions[i];

        // Calculate legal move number for each region
        // Poison legal moves are treated as negative legal moves
        // Perhaps X-squares should only be subtracted once, since a sacrifice of a 
        // corner along an X-square is more common than a sacrifice along an edge.
        int lmNumBlack = PC(lmBlack & region);
        int lmNumWhite = PC(lmWhite & region);
        int total = lmNumBlack + lmNumWhite; // Total should not be affected by poison squares
        if (total == 0) continue;
        lmNumBlack -= 2 * PC(lmBlack & region & poisonBlack);
        lmNumWhite -= 2 * PC(lmWhite & region & poisonWhite);

        // Calculations (no explanations for these yet, but we use an exponential)
        int diff = lmNumBlack - lmNumWhite;
        int absDiff = (diff > 0) ? diff : -diff;
        double exponent = int_pow(TENTH_POWER_OF_FOUR, 35 - total);
        double score = 1 - int_pow(1 - 0.01 * absDiff, exponent); 
        score = (diff > 0) ? score : -score;
        if (PC(region & CORNERS) == 1) score *= 1.5;
        int factor = 2.1 - 0.1 * PC(region);
        factor = (factor > 1) ? factor : 1;
        score *= factor;
        eeRM += score;
    }
    eeRM /= numRegions;

    // Putting it all together
    double cornerWeight = 0.55;
    double remainingWeight = 1 - cornerWeight;
    double frontierWeight = 0.3 * remainingWeight;
    double mobilityWeight = 0.4 * remainingWeight;
    double rmWeight = 0.3 * remainingWeight;
    printf("frontier: %f\n", frontierWeight * eeF);
    printf("mobility: %f\n", mobilityWeight * eeM);
    printf("corners: %f\n", cornerWeight * eeC);
    printf("regional mobility %f\n", rmWeight * eeRM);
    printf("Actual eval: %d\n", eval(black, white));
}

void printEval2(u64 black, u64 white) {
    u64 taken = black | white;
    u64 empty = ~taken;
    // Frontier
    // Idea: double frontier (2 away and unshielded)?
    // Old idea: simple frontier ratio
    //double blackFrontierRatio = PC(frontier(black, white)) / (double) PC(black);
    //double whiteFrontierRatio = PC(frontier(white, black)) / (double) PC(white);
    // New idea: Weighted frontiers
    double eeF = frontierScore(white, black) - frontierScore(black, white);

    // Corners
    double eeC = (PC(black & CORNERS) - PC(white & CORNERS));
    // Score for corner patterns
    eeC += SCP(A1 | B1);
    eeC += SCP(A1 | A2);
    eeC += SCP(H1 | G1);
    eeC += SCP(H1 | H2);
    eeC += SCP(A8 | B8);
    eeC += SCP(A8 | A7);
    eeC += SCP(H8 | G8);
    eeC += SCP(H8 | H7);
    eeC += SCP(A1 | B1 | C1);
    eeC += SCP(A1 | A2 | A3);
    eeC += SCP(H1 | G1 | F1);
    eeC += SCP(H1 | H2 | H3);
    eeC += SCP(A8 | B8 | C8);
    eeC += SCP(A8 | A7 | A6);
    eeC += SCP(H8 | G8 | F8);
    eeC += SCP(H8 | H7 | H6);

    // Poison squares (squares that give up corner, or risky x-squares)
    // This computation is finished in the C-squares section
    u64 poisonBlack = 0;
    u64 poisonWhite = 0;
    // X-square check
    XSQC(A1, B2);
    XSQC(H1, G2);
    XSQC(A8, B7);
    XSQC(H8, G7);
    // Empty c-square check
    EMPTY_CSQ(A1, B1, C1);
    EMPTY_CSQ(A1, A2, A3);
    EMPTY_CSQ(H1, H2, H3);
    EMPTY_CSQ(H1, G1, F1);
    EMPTY_CSQ(A8, B8, C8);
    EMPTY_CSQ(A8, A7, A6);
    EMPTY_CSQ(H8, H7, H6);
    EMPTY_CSQ(H8, G8, F8);

    // C squares
    // More isolated c-squares are worse
    // Macro for modifying c-square score based on corner a and adjacent c-square b
    CSQS(A1, B1, SHIFT_RIGHT);
    CSQS(A1, A2, SHIFT_DOWN);
    CSQS(H1, G1, SHIFT_LEFT);
    CSQS(H1, H2, SHIFT_DOWN);
    CSQS(A8, B8, SHIFT_RIGHT);
    CSQS(A8, A7, SHIFT_UP);
    CSQS(H8, G8, SHIFT_LEFT);
    CSQS(H8, H7, SHIFT_UP);

    // Scale down corner and c-square scores
    eeC /= (1 + PC(EDGES & taken)) / 2.4;
    
    // Mobility
    u64 lmBlack = findLegalMoves(black, white);
    u64 lmWhite = findLegalMoves(white, black);
    int lmNumBlack = PC(lmBlack);
    int lmNumWhite = PC(lmWhite);
    int diff = lmNumBlack - lmNumWhite - 1; // The -1 is a penalty for having to move
    int absDiff = (diff > 0) ? diff : -diff;
    double exponent = int_pow(TENTH_POWER_OF_FOUR, 35 - lmNumBlack - lmNumWhite);
    double eeM = 1 - int_pow(1 - 0.01 * absDiff, exponent); // No explanation for this formula yet, still testing
    eeM = (diff > 0) ? eeM : -eeM;

    // Regional mobility
    double eeRM = 0;
    u64 regions[15];
    u8 numRegions = findRegions(~(black | white), regions);
    for (int i = 0; i < numRegions; i++) {
        u64 region = regions[i];

        // Calculate legal move number for each region
        // Poison legal moves are treated as negative legal moves
        // Perhaps X-squares should only be subtracted once, since a sacrifice of a 
        // corner along an X-square is more common than a sacrifice along an edge.
        int lmNumBlack = PC(lmBlack & region);
        int lmNumWhite = PC(lmWhite & region);
        int total = lmNumBlack + lmNumWhite; // Total should not be affected by poison squares
        if (total == 0) continue;
        lmNumBlack -= 2 * PC(lmBlack & region & poisonBlack);
        lmNumWhite -= 2 * PC(lmWhite & region & poisonWhite);

        // Calculations (no explanations for these yet, but we use an exponential)
        int diff = lmNumBlack - lmNumWhite;
        int absDiff = (diff > 0) ? diff : -diff;
        double exponent = int_pow(TENTH_POWER_OF_FOUR, 35 - total);
        double score = 1 - int_pow(1 - 0.01 * absDiff, exponent); 
        score = (diff > 0) ? score : -score;
        if (PC(region & CORNERS) == 1) score *= 1.5;
        int factor = 2.1 - 0.1 * PC(region);
        factor = (factor > 1) ? factor : 1;
        score *= factor;
        eeRM += score;
    }
    eeRM /= numRegions;

    // Putting it all together
    double cornerWeight = 0.55;
    double remainingWeight = 1 - cornerWeight;
    double frontierWeight = 0.3 * remainingWeight;
    double mobilityWeight = 0.4 * remainingWeight;
    double rmWeight = 0.3 * remainingWeight;
    fprintf(stderr, "frontier: %f\n", frontierWeight * eeF);
    fprintf(stderr, "mobility: %f\n", mobilityWeight * eeM);
    fprintf(stderr, "corners: %f\n", cornerWeight * eeC);
    fprintf(stderr, "regional mobility %f\n", rmWeight * eeRM);
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
