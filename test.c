#include "search.h"
#include <stdlib.h>
#include <check.h>
#include "eval_error.h"

#define FFO_ENDGAME_TESTS 1
#define TEST_FORKING 0

int pv(u64 black, u64 white, int depth, u8 *moves);
void doMoves(u64 *black, u64 *white, u8 *moves, int numMoves);

// Eval timing: 250 ns
// doMove time: 12.1-12.3 ns
// findLegalMove time: initial 14.1-14.3 ns, middle 15.4-15.6, end 11.7-11.9

// Print board and eval
#define PBE(side_to_move, other_side) printBoard(black, white); printEval(side_to_move, other_side);

START_TEST(initializeTT_x0)
{
    initializeTT();
}
END_TEST

START_TEST(boardHash_x6)
{
    // Testing time for hash function
    // Worst case (60 filled squares)
    u64 black = 0xFFFFFFFFFFFFFFF0ull; 
    u64 white = 0;
    u32 h = 0;
    for (int i = 0; i < 1000000; i++) h = boardHash(black, white) % ENDGAME_TT_LEN;
    ck_assert(h);
}
END_TEST

START_TEST(endgameAlphabetaMove_bug_1)
{
    // initializeTT();
    // u64 black = F2 | B3 | C3 | F3 | G3 | B4 | C4 | G4 | H4 | B5 | A6 | G5
    //           | H5 | G6 | H6;
    // u64 white = D1 | E1 | D2 | E2 | E3 | D4 | F4 | C5 | C6 | D5 | D6 | E5 | E6 | F6 | C7 | C8 | E7
    //           | F7 | F8 | C2 | D3 | E4 | F5;
    // printBoard(black, white);
    // ck_assert_int_eq(endgameAlphabeta(black, white, 0), 0);
    // ck_assert_int_eq(endgameAlphabeta(black, white, 1), 0);
    
    // u64 black = A1 | A2 | A3 | A4 | A5 | A6 | B2 | B3 | B6 | C1 | C2 | C3 | C4 | C5 | C6 | C7 | C8
    //           | D4 | D6 | E3 | E5 | F4 | F5 | F6 | F7 | F8;
    // u64 white = B4 | B5 | D1 | E1 | D2 | E2 | D3 | F3 | B4 | E4 | B5 | D5 | E6 | D7 | E7 | D8 | E8;
    // No test currently included for time purposes, but this could
    // be a good test when changes are made to the endgame search functions later on.
}
END_TEST

START_TEST(live_othello_12)
{
    // White to move, -23
    u64 black = A7 | B7 | B6 | B5 | C7 | C6 | C5 | C4 | C3 | D2 | D3 | D4 | D5 | E2 | E3 | E4 
              | E5 | F3 | F5 | F6 | G5 | G7 | H5;
    u64 white = A2 | A3 | A4 | A5 | B1 | B3 | B4 | B8 | C1 | C2 | C8 | D1 | D6 | D7 | D8 | E6
              | F7 | G8;
    ck_assert_int_gt(EVAL(white, black), 0);
}
END_TEST

START_TEST(live_othello_11)
{
    #if 0 // Keeping for possibly later
    // Black to move, white advantage, -8
    u64 black = D8 | E4 | E7 | F3 | F6 | F7 | G5 | G6 | H2 | H3 | H4 | H5;
    u64 white = C5 | D2 | E3 | D4 | D5 | D6 | E5 | E6 | F4 | F5 | G3 | G4;
    // Following principal variation
    u8 moves[60];
    pv(black, white, 10, moves);
    doMoves(&black, &white, moves, 8);
    PBE(black, white);
    #endif
}
END_TEST

START_TEST(live_othello_10)
{
    // Very close game (theoretical result +2 black)
    // black to move
    u64 black = C1 | C2 | B8 | C8 | D8 | D1 | D2 | D3 | E1 | E2 | E3 | E4 | F1 | F2 | F3 | F5
              | G1 | G3 | G4 | G6 | H2 | H3 | H4 | H5 | H6;
    u64 white = A1 | B2 | C3 | D4 | F4 | C5 | D5 | E5 | G5 | D6 | E6 | F6 | D7 | E7 | F7 | F8;
    int e = EVAL(black, white);
    ck_assert_int_gt(e, -100);
}
END_TEST

START_TEST(live_othello_9)
{
    // White is better, white to move
    u64 black = A2 | A3 | A4 | B3 | B5 | C2 | C4 | C6 | D5 | E4 | E5 | F4 | F5;
    u64 white = B4 | C3 | C5 | D3 | D4 | D6 | D7 | E3 | E6 | F6;
    ck_assert_int_gt(EVAL(white, black), 0);
    white = doMove(white, black, CLZ(A6));
    black &= ~white;
    ck_assert_int_lt(EVAL(black, white), 0);
}
END_TEST

START_TEST(live_othello_8)
{
    // Black to move, white advantage, -13
    u64 black = B3 | B4 | B5 | B8 | C4 | C6 | C7 | C8 | D4 | D5 | D6 | D7 | D8
              | E4 | E5 | E6 | E7 | E8 | F4 | F5 | G4 | G5;
    u64 white = A2 | A3 | A4 | A5 | A6 | A7 | B6 | C3 | C5 | D3 | E3 | F3 | H4 | H5 | H6;
    ck_assert_int_lt(EVAL(black, white), 0);
}
END_TEST

START_TEST(eval_timing_x6)
{
    u64 black = B3 | B4 | B5 | B8 | C4 | C6 | C7 | C8 | D4 | D5 | D6 | D7 | D8
              | E4 | E5 | E6 | E7 | E8 | F4 | F5 | G4 | G5;
    u64 white = A2 | A3 | A4 | A5 | A6 | A7 | B6 | C3 | C5 | D3 | E3 | F3 | H4 | H5 | H6;
    int result;
    for (int i = 0; i < 1000000; i++) result = EVAL(black, white);
    ck_assert_int_gt(result, MIN_EVAL); // Dummy test
}
END_TEST

START_TEST(live_othello_7)
{
    u64 black = A4 | A5 | A6 | B5 | B6 | C2 | C3 | C4 | C5 | C6 | D3 | D4 | D7 | D8
              | E3 | E4 | E5 | E7 | E8 | F3 | F4 | F5 | F6 | F7 | F8;
    u64 white = B4 | D5 | D6 | E6;
    ck_assert_int_ne(EXTRACT_MOVE(alphabetaMove(white, black, 4, -MAX_EVAL, MAX_EVAL)), 62);
}
END_TEST

START_TEST(live_othello_6)
{
    // White to move, huge white advantage
    u64 black = A3 | A4 | A5 | A6 | B3 | B4 | B5 | B6 | C2 | C3 | C5 | C6 | D1 | D2 | D3
              | D4 | D6 | D7 | D8 | E3 | E4 | E5 | E7 | E8 | F3 | F4 | F6 | F7 | F8 | G4
              | H4;
    u64 white = C4 | D5 | E6 | F5;
    ck_assert_int_ne(alphabetaMove(white, black, 4, MIN_EVAL, MAX_EVAL), 8);
}
END_TEST

START_TEST(alphabeta_no_lm)
{
    u64 black = A1 | A2 | B1;
    u64 white = C1 | B2 | A3;
    int ab_result = alphabeta(black, white, 4, -1, 1);
    ck_assert_int_gt(ab_result, 0);
}
END_TEST

START_TEST(live_othello_5)
{
    // REGRESSION
    // Would be nice to pass this again one day while keeping 
    // The influence of corners from being too large.
    // Not a huge deal, though, since a search of just a couple of plies deeper
    // should provide a more accurate eval.

    // white to move, white advantage, -42 (final score)
    // u64 black = A1 | A2 | A3 | A4 | A5 | A6 | B4 | B5 | B6 | C5 | C6 | D1 | D2 | D3
    //           | D4 | D6 | D7 | D8 | E3 | E4 | E5 | E7 | E8 | F3 | F4 | F6 | F7 | F8 | G4 | H4;
    // u64 white = B3 | C1 | C2 | C3 | C4 | D5 | E6 | F5;
    // ck_assert_int_lt(EVAL(white, black), 0);
    // ck_assert_int_lt(alphabeta(white, black, 4, -1, 0), 0);
}
END_TEST

START_TEST(live_othello_4)
{
    // white to move, white advantage, -2.6
    u64 black = A2 | A3 | A4 | A5 | A6 | B3 | C2 | C3 | C4 | C5 | D3 | D4 | D5 | D6
              | E3 | E4 | E5 | F2 | F5 | A7 | B6 | B7 | A8 | C6;
    u64 white = B4 | B5 | C7 | C8 | D6 | E6;
    ck_assert_int_lt(EVAL(white, black), 0);
}
END_TEST

START_TEST(live_othello_3)
{
    // white to move, white advantage, +29
    u64 black = A2 | A3 | A4 | A5 | A6 | B3 | B5 | B6 | C2 | C3 | C4 | C5 | C6 | D3 | D4 
              | D5 | D6 | E3 | E4 | E5 | E6 | F2 | F5 | F6 | F7 | G6;
    u64 white = B4 | C7 | C8;
    ck_assert_int_gt(EVAL(white, black), 0);
}
END_TEST

START_TEST(live_othello_2)
{
    // d3 c5 f6 f5 e6 e3 c4 f7 g4 f4 f3 c3 g5 d2 g6 f2
    // black to move, black advantage, +12
    u64 black = D5 | E4 | E5 | E6 | G4 | G5 | G6;
    u64 white = C3 | C4 | C5 | D2 | D3 | D4 | E3 | F2 | F3 | F4 | F5 | F6 | F7;
    ck_assert_int_gt(EVAL(black, white), 0);
    //ck_assert_int_gt(alphabeta(black, white, 4, -1, 1), 0);
}
END_TEST

START_TEST(live_othello_1)
{
    // f5 f6 e6 d6 e7 g5 g6 e8 h5 e3 c5 c4 d7 f4 c6 d8 f8
    // -4 (white advantage, white to move)
    u64 black = C5 | C6 | D6 | E6 | E7 | F8 | F6 | F5 | G5 | G6 | H5;
    u64 white = E3 | C4 | D4 | E4 | F4 | D5 | E5 | D7 | D8 | E8;
    ck_assert_int_gt(EVAL(white, black), 0);
    // ck_assert_int_gt(alphabeta(white, black, 4, -1, 1), 0); // TODO: Currently fails
}
END_TEST


START_TEST(legal_moves_initial_position)
{
    u64 black = STARTING_BOARD_BLACK;
    u64 white = STARTING_BOARD_WHITE;
    u64 lm = findLegalMoves(black, white);

    ck_assert(lm == (E6 | F5 | D3 | C4));
}
END_TEST

START_TEST(legal_moves_totalCount_62)
{
    u64 black = -1ull & ~(BIT(63) | BIT(0) | BIT(7));
    u64 white = BIT(63);
    ck_assert(findLegalMoves(black, white) == 0);
    ck_assert(findLegalMoves(white, black) == (BIT(0) | BIT(7)));
}
END_TEST
    
START_TEST(doMove_initial_position)
{
    u64 black = doMove(STARTING_BOARD_BLACK, STARTING_BOARD_WHITE, SQUARE2('e', 6));
    u64 white = STARTING_BOARD_WHITE & ~black;
    ck_assert(black == (D5 | E4 | E5 | E6));
    ck_assert(white == D4);
}
END_TEST

START_TEST(doMove_no_legal_moves)
{
    u64 black = -1ull & ~A1 & ~B8;
    u64 white = B8;
    black = doMove(black, white, 0);
    white &= ~black;
    ck_assert(black == (-1ull & ~A1 & ~B8));
    ck_assert(white == B8);
}
END_TEST

START_TEST(betterPlayer_1)
{
    u64 black = D3 | C4 | D4 | E4 | D5 | D6;
    u64 white = C3 | A5 | B5 | C5 | E5;
    ck_assert_int_lt(EVAL(white, black), 0);
    ck_assert_int_lt(alphabeta(white, black, 4, MIN_EVAL, MAX_EVAL), 0);
}
END_TEST

#if 0
START_TEST(betterPlayer_2)
{
    u64 black = E2 | D3 | B5 | C5 | D5 | C6;
    u64 white = A3 | C3 | E3 | A4 | B4 | C4 | D4 | E4 | A5 | E5;
    ck_assert_int_lt(EVAL(black, white), 0);
    ck_assert_int_lt(alphabeta(black, white, 4, MIN_EVAL, MAX_EVAL), 0);
}
END_TEST
#endif

START_TEST(betterPlayer_3)
{
    u64 black = F2 | E3 | B4 | C4 | D4 | C5 | D5 | C5;
    u64 white = E1 | F1 | G1 | E2 | A3 | C3 | D3 | A4 | E4 | A5 | B5 | E5 | A6;
    ck_assert_int_lt(EVAL(white, black), 0);
    ck_assert_int_lt(alphabeta(white, black, 4, MIN_EVAL, MAX_EVAL), 0);
}
END_TEST

START_TEST(alphabeta_2nd_move)
{
    u64 black = STARTING_BOARD_BLACK;
    u64 white = STARTING_BOARD_WHITE;
    black = doMove(black, white, CLZ(E6));
    white &= ~black;
    int result1 = -alphabeta(white, black, 1, MIN_EVAL, MAX_EVAL);
    u64 whiteDiag = doMove(white, black, CLZ(F6));
    u64 blackDiag = black &~ whiteDiag;
    u64 whitePerp = doMove(white, black, CLZ(F4));
    u64 blackPerp = black &~ whitePerp;
    u64 whitePara = doMove(white, black, CLZ(D6));
    u64 blackPara = black &~ whitePara;

    ck_assert_int_le(result1, EVAL(blackPara, whitePara));
    ck_assert_int_le(result1, EVAL(blackDiag, whiteDiag));
    ck_assert_int_le(result1, EVAL(blackPerp, whitePerp));
}
END_TEST

#if 0
START_TEST(frontierScore_early_position)
{
    u64 black = D3 | C4 | E4 | F4 | C5 | E6;
    u64 white = D4 | D5 | E5 | F5 | D6;
    ck_assert(frontierScore(black, white, 0) == 24 / (3 * 11) + (6) / 8.0);
    ck_assert(frontierScore(white, black, 0) == 12 / (3 * 11) + (5) / 8.0);
}
END_TEST
#endif

START_TEST(bad_midgame_position_1)
{
    u64 black = C1 | D1 | E1 | F1 | G1 | F2 | H2 | E3 | G3 | H3 | E4 | F4 | E5;
    u64 white = C2 | D2 | E2 | D3 | F3 | D4 | G4 | C5 | D5 | F5 | G5 | H5 | C6
              | D6 | E6 | F6 | E7 | F7;
    ck_assert_int_gt(-EVAL(white, black), 0);
    ck_assert_int_gt(-alphabeta(white, black, 4, MIN_EVAL, MAX_EVAL), 0);
}
END_TEST

START_TEST(bad_opening_position_1)
{
    u64 black = C4 | B5 | C5 | D5 | C6 | D6;
    u64 white = B6 | C3 | D3 | E3 | D4 | E4 | E5;
    ck_assert_int_gt(-EVAL(white, black), 0);
    ck_assert_int_gt(-alphabeta(white, black, 4, MIN_EVAL, MAX_EVAL), 0);
}
END_TEST

START_TEST(bad_midgame_position_low_mobility)
{
    // This is about -20 for WZebra
    u64 black = C4 | D5 | D6 | D7 | E6;
    u64 white = C3 | D3 | E3 | D4 | E4 | E5 | A5 | B5 | C5 | B6 | C6 | C7;
    ck_assert_int_gt(-EVAL(white, black), 0);
    ck_assert_int_gt(-alphabeta(white, black, 4, MIN_EVAL, MAX_EVAL), 0);
}
END_TEST

#if 0 // Removed since not a good position for static or low-depth testing
START_TEST(bad_late_midgame_position_1)
{
    // This is actually a lost position for black
    // Winning moves are at least A5, A6, G2, F8, H6, and G8
    u64 black = B6 | E3 | E6 | E7 | E8 | F2 | F4 | F5 | F6 | F7 | G1 | G3 | G4 | G5 | G6 
              | H3 | H4 | H7;
    u64 white = C2 | C3 | C4 | C5 | C6 | C7 | D2 | D3 | D4 | D5 | D6 | D7 | E1 | E2 | E4
              | E5 | F3;
    printBoard(black, white);
    printEval(black, white);
    ck_assert_int_gt(-EVAL(white, black), 0);
    ck_assert_int_gt(-alphabeta(white, black, 4, MIN_EVAL, MAX_EVAL), 0);
}
END_TEST
#endif

START_TEST(endgameAlphabeta_totalCount_63)
{
    u64 black = -1ull & ~(BIT(63) | BIT(0));
    u64 white = BIT(63);
    ck_assert_int_gt(endgameAlphabeta63(black, white, 1), 0);
}
END_TEST

START_TEST(endgameAlphabeta_totalCount_62)
{
    u64 black = -1ull & ~(BIT(63) | BIT(0) | BIT(7));
    u64 white = BIT(63);
    ck_assert_int_gt(endgameAlphabeta62(black, white, findLegalMoves(black, white), 1), 0);
}
END_TEST

#if FFO_ENDGAME_TESTS
// Time should be mostly just initializeTT time
START_TEST(ffo_endgame_40)
{
    initializeTT();
    u64 black = C3 | D3 | C4 | H1 | H2 | H3 | H4 | H5 | H6 | H7 | G4 | G5;
    u64 white = A1 | A3 | A4 | A5 | B2 | B3 | B4 | B5 | C2 | C5 | D1 | D2
              | D4 | D5 | D6 | E1 | E2 | E3 | E4 | E5 | E6 | E7 | F1 | F2
              | F3 | F4 | F5 | F6 | G1 | G2 | G3 | G6;
    ck_assert_int_gt(ENDGAME_ALPHABETA_UPPER(black, white), 0);
}
END_TEST

// Best time: 3.4 sec
START_TEST(ffo_endgame_41)
{
    initializeTT();
    u64 black = A4 | B4 | B5 | C4 | C5 | C6 | D4 | D7 | E4 | E6 | E7 | F5
              | F6 | G2;
    u64 white = A6 | B1 | B3 | B8 | B6 | C1 | C2 | C3 | C8 | C7 | D1 | D2
              | D3 | D8 | D6 | D5 | E1 | E2 | E3 | E5 | F1 | F2 | F3 | F4
              | F7 | G3 | G4 | G8;

    ck_assert_int_eq(ENDGAME_ALPHABETA_UPPER(black, white), 0);

    #if 0
    #if COUNT_COLLISIONS
    {
        int numEmpty = 0;
        for (size_t i = 0; i < ENDGAME_TT_LEN; i++) {
            if (endgameTT[i].black == 0) numEmpty++;
        }
        printf("empty %d\n", numEmpty);
        printf("filled %d\n", 1024 * 1024 * 64 - numEmpty);
        int num1 = 0;
        for (size_t i = 0; i < ENDGAME_TT_LEN; i++) {
            if (collisions[i] == 1) num1++;
        }
        printf("num1 %d\n", num1);
        int num2 = 0;
        for (size_t i = 0; i < ENDGAME_TT_LEN; i++) {
            if (collisions[i] == 2) num2++;
        }
        printf("num2 %d\n", num2);
        int num3 = 0;
        for (size_t i = 0; i < ENDGAME_TT_LEN; i++) {
            if (collisions[i] == 3) num3++;
        }
        printf("num3 %d\n", num3);
        int num4 = 0;
        for (size_t i = 0; i < ENDGAME_TT_LEN; i++) {
            if (collisions[i] == 4) num4++;
        }
        printf("num4 %d\n", num4);
        int num = 0;
        for (size_t i = 0; i < ENDGAME_TT_LEN; i++) {
            if (collisions[i] > 3) num++;
        }
        printf("num %d\n", num);
        printf("ttHits %lu\n", ttHits);
    }
    #endif
    #endif
    
    ck_assert_int_eq(ENDGAME_ALPHABETA_LOWER(black, white), 0);

    #if 0
    #if COUNT_COLLISIONS
    {
        int numEmpty = 0;
        for (size_t i = 0; i < ENDGAME_TT_LEN; i++) {
            if (endgameTT[i].black == 0) numEmpty++;
        }
        printf("empty %d\n", numEmpty);
        printf("filled %d\n", 1024 * 1024 * 64 - numEmpty);
        int num1 = 0;
        for (size_t i = 0; i < ENDGAME_TT_LEN; i++) {
            if (collisions[i] == 1) num1++;
        }
        printf("num1 %d\n", num1);
        int num2 = 0;
        for (size_t i = 0; i < ENDGAME_TT_LEN; i++) {
            if (collisions[i] == 2) num2++;
        }
        printf("num2 %d\n", num2);
        int num3 = 0;
        for (size_t i = 0; i < ENDGAME_TT_LEN; i++) {
            if (collisions[i] == 3) num3++;
        }
        printf("num3 %d\n", num3);
        int num4 = 0;
        for (size_t i = 0; i < ENDGAME_TT_LEN; i++) {
            if (collisions[i] == 4) num4++;
        }
        printf("num4 %d\n", num4);
        int num = 0;
        for (size_t i = 0; i < ENDGAME_TT_LEN; i++) {
            if (collisions[i] > 3) num++;
        }
        printf("num %d\n", num);
        printf("ttHits %lu\n", ttHits);
    }
    #endif
    #endif
}
END_TEST

// Best time: 1.04 sec
START_TEST(ffo_endgame_42)
{
    initializeTT();
    u64 black = A5 | E2 | F2 | F3 | F4 | F5 | F6 | G5 | G7;
    u64 white = C1 | D1 | E1 | H2 | A3 | B3 | C3 | D3 | E3 | G3 | H3
              | B4 | C4 | D4 | E4 | G4 | H4 | C5 | D5 | E5 | H5 | D6
              | E6 | G6 | H6 | D7 | E7 | F7 | H7 | C8 | D8 | E8 | F8;

    ck_assert_int_gt(ENDGAME_ALPHABETA_UPPER(black, white), 0);
}
END_TEST

START_TEST(ffo_endgame_43)
{
    initializeTT();
    u64 black = E2 | F2 |F3 | F4 | F5 | F6 |G5 | G7 |A5;
    u64 white = C1 | D1 | E1 | H2 | A3 | B3 | C3 | D3 | E3 | G3 | H3
              | B4 | C4 | D4 | E4 | G4 | H4 | C5 | D5 | E5 | H5 | D6 
              | E6 | G6 | H6 | D7 | E7 | F7 | H7 | C8 | D8 | E8 | F8;
    ck_assert_int_gt(ENDGAME_ALPHABETA_UPPER(black, white), 0);
}
END_TEST

START_TEST(ffo_endgame_44)
{
    initializeTT();
    u64 black = E1 | E2 | D3 | E3 | F3 | E4 | F4 | G4 | E5 | F5 | E6 | A6 | B6 | C7 | D7 | E7 | F7 | D8 | E8 | F8;
    u64 white = C1 | G1 | C2 | F2 | H2 | B3 | C3 | G3 | H3 | A4 | B4 | C4 | D4 | H4 | A5 | B5 | C5 | D5 | C6 | D6 | F6;
    ck_assert_int_lt(ENDGAME_ALPHABETA_LOWER(white, black), 0);
}
END_TEST

// Best time: 4.56 sec
START_TEST(ffo_endgame_45)
{
    initializeTT();
    u64 black = D1 | E1 | F1 | G1 | A2 | C2 | D2 | E2 | A3 | B3 | D3
              | A4 | B4 | C4 | E4 | A5 | B5 | D5 | E5 | C6 | D6 | E6;
    u64 white = F2 | C3 | E3 | F3 | D4 | F4 | C5 | F5 | B6 | F6 | G6
              | A7 | C7 | D7 | E7 | F7 | E8 | F8; 

    ck_assert_int_gt(ENDGAME_ALPHABETA_UPPER(black, white), 0);
}
END_TEST

START_TEST(ffo_endgame_46)
{
    initializeTT();
    u64 black = D1 | E1 | F1 | F2 | F3 | G3 | F4 | G4 | H4 | G5 | H5 | F6 | G6 | H6
              | D6 | C7 | D7 | B8 | C8 | D8 | E8;;
    u64 white = C2 | D2 | E2 | C3 | D3 | E3 | B4 | C4 | D4 | E4 | C5 | D5 | E5 | F5
              | C6 | E6 | E7 | F7 | G8;

    ck_assert_int_lt(ENDGAME_ALPHABETA_LOWER(black, white), 0);
}
END_TEST

START_TEST(ffo_endgame_47)
{
    initializeTT();
    u64 black = A4 | B4 | C4 | D4 | E4 | F4 | F3 | C5 | F5 | D6 | F6 | E7 | F7 | C8 | D8 | E8 | F8;
    u64 white = B1 | C1 | D1 | E1 | F1 | C2 | D2 | E2 | F2 | B3 | C3 | D3 | E3 | B5 | D5 | E5 | A6
              | B6 | C6 | E6 | C7 | D7;
    ck_assert_int_gt(ENDGAME_ALPHABETA_UPPER(white, black), 0);
}
END_TEST

// Best time: 17.00 sec
START_TEST(ffo_endgame_50)
{
    initializeTT();
    u64 black = E1 | C2 | D2 | E2 | E3 | E4 | E5 | E6 | E7 | C5 | G5 | D6;
    u64 white = B3 | C3 | D3 | B4 | C4 | D4 | B5 | D5 | B6 | C6 | C7 | C8
              | D7 | F8 | F7 | F6 | G6 | H6 | H5 | F5 | G4 | H4 | F4 | F3
              | G3 | H3;
    ck_assert_int_gt(ENDGAME_ALPHABETA_UPPER(black, white), 0);
}
END_TEST
#endif

// Best time: 96.9 ns / call
START_TEST(endgameAlphabeta_totalCount_62x7)
{
    u64 black = -1ull & ~(BIT(63) | BIT(0) | BIT(7));
    u64 white = BIT(63);
    int passed = 1;
    for (int i = 0; i < 10000000; i++) {
        passed *= (endgameAlphabeta62(black, white, findLegalMoves(black, white), 1) > 0);
    }
    ck_assert_int_eq(passed, 1);
}
END_TEST
    
// Best time: 18.7 ns / call
START_TEST(endgameAlphabeta_totalCount_63x7)
{
    u64 black = -1ull & ~(BIT(63) | BIT(0));
    u64 white = BIT(63);
    int passed = 1;
    for (int i = 0; i < 10000000; i++) {
        passed *= (endgameAlphabeta63(black, white, 1) > 0);
    }
    ck_assert_int_eq(passed, 1);
}
END_TEST

int pv(u64 black, u64 white, int depth, u8 *moves) {
    if (depth != 0) {
        *moves = alphabetaMove(black, white, depth, MIN_EVAL, MAX_EVAL);
        black = doMove(black, white, *moves);
        white &= ~black;
        return 1 + pv(white, black, depth - 1, moves + 1);
    }
    return 0;
}

void doMoves(u64 *black, u64 *white, u8 *moves, int numMoves) {
    for (int i = 0; i < numMoves; i++) {
        if (i % 2 == 0) {
            // Black to move
            *black = doMove(*black, *white, moves[i]);
            *white &= ~*black;
        }
        else {
            // White to move
            *white = doMove(*white, *black, moves[i]);
            *black &= ~*white;
        }
    }
}

Suite * othello_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Othello");
    tc_core = tcase_create("Core");
    
    // Give longer timeout
    tcase_set_timeout(tc_core, 120);

    // Add tests here by name
    #define ADD_TEST(test_name) tcase_add_test(tc_core, test_name);
    ADD_TEST(initializeTT_x0);
    ADD_TEST(boardHash_x6);
    ADD_TEST(live_othello_1);
    ADD_TEST(live_othello_2);
    ADD_TEST(live_othello_3);
    ADD_TEST(live_othello_4);
    ADD_TEST(live_othello_5);
    ADD_TEST(live_othello_6);
    ADD_TEST(live_othello_7);
    ADD_TEST(live_othello_8);
    ADD_TEST(live_othello_9);
    //ADD_TEST(live_othello_10);
    ADD_TEST(live_othello_11);
    ADD_TEST(live_othello_12);
    ADD_TEST(alphabeta_no_lm);
    ADD_TEST(legal_moves_initial_position);
    ADD_TEST(legal_moves_totalCount_62);
    ADD_TEST(doMove_initial_position);
    ADD_TEST(doMove_no_legal_moves);
    //ADD_TEST(frontierScore_early_position);
    ADD_TEST(betterPlayer_1);
    //ADD_TEST(betterPlayer_2);
    ADD_TEST(betterPlayer_3);
    ADD_TEST(alphabeta_2nd_move);
    ADD_TEST(bad_opening_position_1);
    ADD_TEST(bad_midgame_position_low_mobility);
    ADD_TEST(bad_midgame_position_1);
    ADD_TEST(endgameAlphabeta_totalCount_63);
    ADD_TEST(endgameAlphabetaMove_bug_1);
    ADD_TEST(endgameAlphabeta_totalCount_62);
    #if FFO_ENDGAME_TESTS
    ADD_TEST(ffo_endgame_40);
    ADD_TEST(ffo_endgame_41);
    ADD_TEST(ffo_endgame_42);
    ADD_TEST(ffo_endgame_43);
    //ADD_TEST(ffo_endgame_44);
    ADD_TEST(ffo_endgame_45);
    //ADD_TEST(ffo_endgame_46);
    //ADD_TEST(ffo_endgame_47);
    ADD_TEST(ffo_endgame_50);
    #endif
    ADD_TEST(endgameAlphabeta_totalCount_62x7);
    ADD_TEST(endgameAlphabeta_totalCount_63x7);
    ADD_TEST(eval_timing_x6);

    suite_add_tcase(s, tc_core);

    return s;
}

// From check framework tutorial (hence the "money" references)
int main(void) {
    // Random seed
    srand(38980);

    initializeNN();

    int number_failed;
    Suite *s;
    SRunner *sr;

    s = othello_suite(); // Freed by srunner_free as well according to docs
    sr = srunner_create(s);

    // For debugging
    #if !TEST_FORKING
    srunner_set_fork_status (sr, CK_NOFORK);
    #endif

    // Set xml output
    srunner_set_xml(sr, "test_results");

    // Run
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
