#ifndef __DEFS_H__
#define __DEFS_H__
// defs.h

// Some global preprocessor definitions

// Endgame start
// The number of discs after which the endgame is considered to start
// endgameAlphabeta should not be called at an earlier depth.
#define ENDGAME_START 38

// 64 bit integer
#define u64 uint64_t

// 8 bit integer
#define u8 uint8_t

// 16 bit integer
#define u16 uint16_t

// 32 bit integer
#define u32 uint32_t

// Quick allocation for u64 pointer
// "Malloc unsigned 64-bit"
#define MU64(x) ((u64 *) malloc(sizeof(u64) * (x)))
#define CU64(x) ((u64 *) calloc((x), sizeof(u64)))
#define MU32(x) ((u32 *) malloc(sizeof(u32) * (x)))
#define CU32(x) ((u32 *) calloc((x), sizeof(u32)))
#define MU8(x) ((u8 *) malloc(sizeof(u8) * (x)))
#define CU8(x) ((u8 *) calloc((x), sizeof(u8)))

// Popcount
#define PC(x) (__builtin_popcountll(x))

// Total count
#define TC(b, w) (PC(b) + PC(w))

// Disc difference (final score)
#define DD(b, w) (PC(b) - PC(w))

// Extract first nonzero bit
#define CLZ(x) (__builtin_clzll(x))
#define CTZ(x) (__builtin_ctzll(x))
#define FNB(x) (PLAIN_BIT(CLZ(x)))

// Bit for cell corresponding to square x
// Square 0 is the MSB, square 63 is LSB
#define BIT(x) (1ull << (63 - (x)))

// Standard n-th bit (LSB is 0th bit)
#define PLAIN_BIT(x) (1ull << (x))

// Colors
#define BLACK 1
#define WHITE 0
#define OTHER_SIDE(s) (!(s))

// Get square corresponding to coordinates
// First version uses 0-based numbers as coordinates
// Second version uses standard othello notation
// For example, SQUARE2('c', 2) = 10
#define SQUARE(x, y) (8 * (y) + (x))
#define SQUARE2(letter, number) SQUARE((letter) - 'a', (number) - 1)

// Bit corresponding to square using standard notation
#define A1 BIT(SQUARE2('a', 1))
#define A2 BIT(SQUARE2('a', 2))
#define A3 BIT(SQUARE2('a', 3))
#define A4 BIT(SQUARE2('a', 4))
#define A5 BIT(SQUARE2('a', 5))
#define A6 BIT(SQUARE2('a', 6))
#define A7 BIT(SQUARE2('a', 7))
#define A8 BIT(SQUARE2('a', 8))

#define B1 BIT(SQUARE2('b', 1))
#define B2 BIT(SQUARE2('b', 2))
#define B3 BIT(SQUARE2('b', 3))
#define B4 BIT(SQUARE2('b', 4))
#define B5 BIT(SQUARE2('b', 5))
#define B6 BIT(SQUARE2('b', 6))
#define B7 BIT(SQUARE2('b', 7))
#define B8 BIT(SQUARE2('b', 8))

#define C1 BIT(SQUARE2('c', 1))
#define C2 BIT(SQUARE2('c', 2))
#define C3 BIT(SQUARE2('c', 3))
#define C4 BIT(SQUARE2('c', 4))
#define C5 BIT(SQUARE2('c', 5))
#define C6 BIT(SQUARE2('c', 6))
#define C7 BIT(SQUARE2('c', 7))
#define C8 BIT(SQUARE2('c', 8))

#define D1 BIT(SQUARE2('d', 1))
#define D2 BIT(SQUARE2('d', 2))
#define D3 BIT(SQUARE2('d', 3))
#define D4 BIT(SQUARE2('d', 4))
#define D5 BIT(SQUARE2('d', 5))
#define D6 BIT(SQUARE2('d', 6))
#define D7 BIT(SQUARE2('d', 7))
#define D8 BIT(SQUARE2('d', 8))

#define E1 BIT(SQUARE2('e', 1))
#define E2 BIT(SQUARE2('e', 2))
#define E3 BIT(SQUARE2('e', 3))
#define E4 BIT(SQUARE2('e', 4))
#define E5 BIT(SQUARE2('e', 5))
#define E6 BIT(SQUARE2('e', 6))
#define E7 BIT(SQUARE2('e', 7))
#define E8 BIT(SQUARE2('e', 8))

#define F1 BIT(SQUARE2('f', 1))
#define F2 BIT(SQUARE2('f', 2))
#define F3 BIT(SQUARE2('f', 3))
#define F4 BIT(SQUARE2('f', 4))
#define F5 BIT(SQUARE2('f', 5))
#define F6 BIT(SQUARE2('f', 6))
#define F7 BIT(SQUARE2('f', 7))
#define F8 BIT(SQUARE2('f', 8))

#define G1 BIT(SQUARE2('g', 1))
#define G2 BIT(SQUARE2('g', 2))
#define G3 BIT(SQUARE2('g', 3))
#define G4 BIT(SQUARE2('g', 4))
#define G5 BIT(SQUARE2('g', 5))
#define G6 BIT(SQUARE2('g', 6))
#define G7 BIT(SQUARE2('g', 7))
#define G8 BIT(SQUARE2('g', 8))

#define H1 BIT(SQUARE2('h', 1))
#define H2 BIT(SQUARE2('h', 2))
#define H3 BIT(SQUARE2('h', 3))
#define H4 BIT(SQUARE2('h', 4))
#define H5 BIT(SQUARE2('h', 5))
#define H6 BIT(SQUARE2('h', 6))
#define H7 BIT(SQUARE2('h', 7))
#define H8 BIT(SQUARE2('h', 8))

// Get bit of bitboard at given coordinates
#define GET2(bb, sq) ((bb) & BIT(sq))
#define GET(bb, x, y) GET2(bb, SQUARE(x, y))

// Starting board
#define STARTING_BOARD_BLACK (BIT(SQUARE2('d', 5)) | BIT(SQUARE2('e', 4)))
#define STARTING_BOARD_WHITE (BIT(SQUARE2('e', 5)) | BIT(SQUARE2('d', 4)))

// Filters for findLegalMoves and doMove
#define DOWN_FILTER 0x00FFFFFFFFFFFFFFull
#define RIGHT_FILTER 0x7F7F7F7F7F7F7F7Full
#define DOWN_RIGHT_FILTER 0x007F7F7F7F7F7F7Full
#define DOWN_LEFT_FILTER 0x00FEFEFEFEFEFEFEull
#define UP_FILTER 0xFFFFFFFFFFFFFF00ull
#define LEFT_FILTER 0xFEFEFEFEFEFEFEFEull
#define UP_RIGHT_FILTER 0x7F7F7F7F7F7F7F00ull
#define UP_LEFT_FILTER 0xFEFEFEFEFEFEFE00ull

// Conditional OR (possibly) without branching
#define C_OR(condition, dest, source) (dest) |= -!!(condition) & (source)

// Shift functions
#define SHIFT_RIGHT(x) ((x) >> 1)
#define SHIFT_LEFT(x) ((x) << 1)
#define SHIFT_DOWN(x) ((x) >> 8)
#define SHIFT_UP(x) ((x) << 8)
#define SHIFT_UP_RIGHT(x) ((x) << 7)
#define SHIFT_DOWN_RIGHT(x) ((x) >> 9)
#define SHIFT_UP_LEFT(x) ((x) << 9)
#define SHIFT_DOWN_LEFT(x) ((x) >> 7)

// Edges
#define EDGES 0xFF818181818181FFull

// Corners
#define CORNERS 0x8100000000000081ull

// X-Squares
#define X_SQUARES (B2 | G2 | B7 | G7)

// Maximum eval constant
#define MAX_EVAL 2048
#define MIN_EVAL -MAX_EVAL

// Null move (a pass)
#define NULL_MOVE 64

// Macro to extract move and score from the alphabetaMove functions
#define EXTRACT_EVAL(x) ((x & ~0xFF) / 256)
#define EXTRACT_MOVE(x) (x & 0xFF)

// Special constant for experimental mobility score formula
#define TENTH_POWER_OF_FOUR 1.148698

// Possible endgame evals as scored in transposition table
#define DRAW 0
#define BLACK_WIN 1
#define WHITE_WIN 2
#define NOT_BLACK_WIN 3  // For when alpha = 0, beta = 1, and the eval given is 0.
#define NOT_WHITE_WIN 4  // For when alpha = -1, beta = 0, and the eval given is 0.

// Extracting eval and number of nodes from endgame transposition table
#define EXTRACT_ENDGAME_TT_EVAL(data) (data & 0x7ull)
#define EXTRACT_ENDGAME_TT_NUM_NODES(data) (data >> 61)

// Endgame transposition table size in bytes
#define HASH_LEN 24 // In bits
#define ENDGAME_TT_LEN (1u << HASH_LEN)
#define ENDGAME_TT_SIZE (ENDGAME_TT_LEN * sizeof(ttEntry))

// When to stop hashing
#define STOP_HASHING_TC 59

// When to stop using midgame evaluation function in endgame search
#define STOP_USING_EVAL 52

// Depth of midgame searches used in endgame
#define ENDGAME_AB_DEPTH 1

// Determine whether the number of transposition table collisions should be counted
#define COUNT_COLLISIONS 0

// Describes searches at first 12 depths
#define AB_SCOUT(black, white) \
for (int i = 1; i <= 12; i++) {\
    int result = alphabetaMove(black, white, i, MIN_EVAL, MAX_EVAL);\
    u8 move = EXTRACT_MOVE(result);\
    int ee = EXTRACT_EVAL(result);\
    printf("Depth %d: eval %d, move %u\n", i, ee, move);\
}
#define AB_SCOUT_STDERR(black, white) \
for (int i = 1; i <= 12; i++) {\
    int result = alphabetaMove(black, white, i, MIN_EVAL, MAX_EVAL);\
    u8 move = EXTRACT_MOVE(result);\
    int ee = EXTRACT_EVAL(result);\
    fprintf(stderr, "Depth %d: eval %d, move %u\n", i, ee, move);\
}

// When to start considering a move as so bad it's clearly losing
#define LOSING_EVAL -150

// ...or so good it's clearly winning
#define WINNING_EVAL 150

// Definition to decide if the previous 2 constants should be used in alphabeta search
#define USE_LOSING_EVAL 1

// Macros to work with midgame transposition table's data fields
#define CONSTRUCT_MIDGAME_TT_DATA(eval, alpha, beta, depth) ((((u64) depth) << 48) | (((((u64) beta) + (1u << 15)) & 0xFFFFu) << 32) | (((((u64) alpha) + (1u << 15)) & 0xFFFFu) << 16) | ((((u64) eval) + (1u << 15)) & 0xFFFFu))
#define EXTRACT_MIDGAME_TT_EVAL(data) (((data) & 0xFFFFu) - (1u << 15))
#define EXTRACT_MIDGAME_TT_ALPHA(data) ((((data) >> 16) & 0xFFFFu) - (1u << 15))
#define EXTRACT_MIDGAME_TT_BETA(data) ((((data) >> 32) & 0xFFFFu) - (1u << 15))
#define EXTRACT_MIDGAME_TT_DEPTH(data) (((data) >> 48) & 0x1Fu)

// At what depth to stop checking transposition table
// Must be greater than or equal to ENDGAME_AB_DEPTH
#define STOP_USING_TT_DEPTH 4

#endif
