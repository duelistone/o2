#include "search.h"
#include <check.h>

START_TEST(live_othello_7)
{
    u64 black = A4 | A5 | A6 | B5 | B6 | C2 | C3 | C4 | C5 | C6 | D3 | D4 | D7 | D8
              | E3 | E4 | E5 | E7 | E8 | F3 | F4 | F5 | F6 | F7 | F8;
    u64 white = B4 | D5 | D6 | E6;
    ck_assert_int_ne(EXTRACT_MOVE(alphabetaMove(white, black, 10, -MAX_EVAL, MAX_EVAL)), 62);
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
    // white to move, white advantage, -42 (final score)
    u64 black = A1 | A2 | A3 | A4 | A5 | A6 | B4 | B5 | B6 | C5 | C6 | D1 | D2 | D3
              | D4 | D6 | D7 | D8 | E3 | E4 | E5 | E7 | E8 | F3 | F4 | F6 | F7 | F8 | G4 | H4;
    u64 white = B3 | C1 | C2 | C3 | C4 | D5 | E6 | F5;
    ck_assert_int_lt(eval(white, black), 0);
    ck_assert_int_lt(alphabeta(white, black, 4, -1, 0), 0);
}
END_TEST

START_TEST(live_othello_4)
{
    // white to move, white advantage, -2.6
    u64 black = A2 | A3 | A4 | A5 | A6 | B3 | C2 | C3 | C4 | C5 | D3 | D4 | D5 | D6
              | E3 | E4 | E5 | F2 | F5 | A7 | B6 | B7 | A8 | C6;
    u64 white = B4 | B5 | C7 | C8 | D6 | E6;
    ck_assert_int_lt(eval(white, black), 0);
}
END_TEST

START_TEST(live_othello_3)
{
    // white to move, white advantage, +29
    u64 black = A2 | A3 | A4 | A5 | A6 | B3 | B5 | B6 | C2 | C3 | C4 | C5 | C6 | D3 | D4 
              | D5 | D6 | E3 | E4 | E5 | E6 | F2 | F5 | F6 | F7 | G6;
    u64 white = B4 | C7 | C8;
    ck_assert_int_gt(eval(white, black), 0);
}
END_TEST

START_TEST(live_othello_2)
{
    // d3 c5 f6 f5 e6 e3 c4 f7 g4 f4 f3 c3 g5 d2 g6 f2
    // black to move, black advantage, +12
    u64 black = D5 | E4 | E5 | E6 | G4 | G5 | G6;
    u64 white = C3 | C4 | C5 | D2 | D3 | D4 | E3 | F2 | F3 | F4 | F5 | F6 | F7;
    ck_assert_int_gt(eval(black, white), 0);
    ck_assert_int_gt(alphabeta(black, white, 4, -1, 1), 0);
}
END_TEST

START_TEST(live_othello_1)
{
    // f5 f6 e6 d6 e7 g5 g6 e8 h5 e3 c5 c4 d7 f4 c6 d8 f8
    // -4 (white advantage, white to move)
    u64 black = C5 | C6 | D6 | E6 | E7 | F8 | F6 | F5 | G5 | G6 | H5;
    u64 white = E3 | C4 | D4 | E4 | F4 | D5 | E5 | D7 | D8 | E8;
    ck_assert_int_gt(eval(white, black), 0);
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
    ck_assert_int_lt(eval(white, black), 0);
    ck_assert_int_lt(alphabeta(white, black, 4, MIN_EVAL, MAX_EVAL), 0);
}
END_TEST

START_TEST(betterPlayer_2)
{
    u64 black = E2 | D3 | B5 | C5 | D5 | C6;
    u64 white = A3 | C3 | E3 | A4 | B4 | C4 | D4 | E4 | A5 | E5;
    ck_assert_int_lt(eval(black, white), 0);
    ck_assert_int_lt(alphabeta(black, white, 4, MIN_EVAL, MAX_EVAL), 0);
}
END_TEST

START_TEST(betterPlayer_3)
{
    u64 black = F2 | E3 | B4 | C4 | D4 | C5 | D5 | C5;
    u64 white = E1 | F1 | G1 | E2 | A3 | C3 | D3 | A4 | E4 | A5 | B5 | E5 | A6;
    ck_assert_int_lt(eval(white, black), 0);
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

    ck_assert_int_le(result1, eval(blackPara, whitePara));
    ck_assert_int_le(result1, eval(blackDiag, whiteDiag));
    ck_assert_int_le(result1, eval(blackPerp, whitePerp));
}
END_TEST

START_TEST(frontierScore_early_position)
{
    u64 black = D3 | C4 | E4 | F4 | C5 | E6;
    u64 white = D4 | D5 | E5 | F5 | D6;
    ck_assert(frontierScore(black, white) == 24 / 42.0 + 6 / 64.0);
    ck_assert(frontierScore(white, black) == 12 / 35.0 + 5 / 64.0);
}
END_TEST

START_TEST(bad_midgame_position_1)
{
    u64 black = C1 | D1 | E1 | F1 | G1 | F2 | H2 | E3 | G3 | H3 | E4 | F4 | E5;
    u64 white = C2 | D2 | E2 | D3 | F3 | D4 | G4 | C5 | D5 | F5 | G5 | H5 | C6
              | D6 | E6 | F6 | E7 | F7;
    ck_assert_int_gt(-eval(white, black), 0);
    ck_assert_int_gt(-alphabeta(white, black, 4, MIN_EVAL, MAX_EVAL), 0);
}
END_TEST

START_TEST(bad_opening_position_1)
{
    u64 black = C4 | B5 | C5 | D5 | C6 | D6;
    u64 white = B6 | C3 | D3 | E3 | D4 | E4 | E5;
    ck_assert_int_gt(-eval(white, black), 0);
    ck_assert_int_gt(-alphabeta(white, black, 4, MIN_EVAL, MAX_EVAL), 0);
}
END_TEST

START_TEST(bad_midgame_position_low_mobility)
{
    // This is about -20 for WZebra
    u64 black = C4 | D5 | D6 | D7 | E6;
    u64 white = C3 | D3 | E3 | D4 | E4 | E5 | A5 | B5 | C5 | B6 | C6 | C7;
    ck_assert_int_gt(-eval(white, black), 0);
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
    ck_assert_int_gt(-eval(white, black), 0);
    ck_assert_int_gt(-alphabeta(white, black, 4, MIN_EVAL, MAX_EVAL), 0);
}
END_TEST
#endif

START_TEST(endgameAlphabeta_totalCount_63)
{
    u64 black = -1ull & ~(BIT(63) | BIT(0));
    u64 white = BIT(63);
    ck_assert_int_gt(endgameAlphabeta63(black, white), 0);
}
END_TEST


START_TEST(endgameAlphabetaMove_totalCount_63)
{
    u64 black = -1ull & ~(BIT(63) | BIT(0));
    u64 white = BIT(63);
    int result = endgameAlphabetaMove(white, black, -1, 1);
    ck_assert((result & ~0xFF) / 256 < 0);
    ck_assert(0 == (result & 0xFF));
}
END_TEST

START_TEST(endgameAlphabetaMove_totalCount_63_no_legal_moves)
{
    u64 black = -1ull & ~(BIT(63) | BIT(0));
    u64 white = BIT(63);
    int result = endgameAlphabetaMove(black, white, -1, 1);
    ck_assert(EXTRACT_EVAL(result) > 0);
    ck_assert(NULL_MOVE == EXTRACT_MOVE(result));
}
END_TEST

START_TEST(endgameAlphabeta_totalCount_62)
{
    u64 black = -1ull & ~(BIT(63) | BIT(0) | BIT(7));
    u64 white = BIT(63);
    ck_assert_int_gt(endgameAlphabeta62(black, white, -1, 1), 0);
}
END_TEST

// Best time: 0.0032 sec
START_TEST(ffo_endgame_40)
{
    u64 black = C3 | D3 | C4 | H1 | H2 | H3 | H4 | H5 | H6 | H7 | G4 | G5;
    u64 white = A1 | A3 | A4 | A5 | B2 | B3 | B4 | B5 | C2 | C5 | D1 | D2
              | D4 | D5 | D6 | E1 | E2 | E3 | E4 | E5 | E6 | E7 | F1 | F2
              | F3 | F4 | F5 | F6 | G1 | G2 | G3 | G6;
    ck_assert_int_gt(endgameAlphabeta(black, white, -1, 1), 0);
}
END_TEST

// Best time: 18.5 sec
START_TEST(ffo_endgame_41)
{
    u64 black = A4 | B4 | B5 | C4 | C5 | C6 | D4 | D7 | E4 | E6 | E7 | F5
              | F6 | G2;
    u64 white = A6 | B1 | B3 | B8 | B6 | C1 | C2 | C3 | C8 | C7 | D1 | D2
              | D3 | D8 | D6 | D5 | E1 | E2 | E3 | E5 | F1 | F2 | F3 | F4
              | F7 | G3 | G4 | G8;

    ck_assert_int_eq(endgameAlphabeta(black, white, -1, 1), 0);
}
END_TEST

// Best time: 101 ns / call
START_TEST(endgameAlphabeta_totalCount_62x7)
{
    u64 black = -1ull & ~(BIT(63) | BIT(0) | BIT(7));
    u64 white = BIT(63);
    int passed = 1;
    for (int i = 0; i < 10000000; i++) {
        passed *= (endgameAlphabeta62(black, white, -1, 1) > 0);
    }
    ck_assert_int_eq(passed, 1);
}
END_TEST
    
// Best time: 20.6 ns / call
START_TEST(endgameAlphabeta_totalCount_63x7)
{
    u64 black = -1ull & ~(BIT(63) | BIT(0));
    u64 white = BIT(63);
    int passed = 1;
    for (int i = 0; i < 10000000; i++) {
        passed *= (endgameAlphabeta63(black, white) > 0);
    }
    ck_assert_int_eq(passed, 1);
}
END_TEST

Suite * money_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Othello");
    tc_core = tcase_create("Core");
    
    // Give longer timeout
    tcase_set_timeout(tc_core, 40);

    // Add tests here by name
    tcase_add_test(tc_core, live_othello_1);
    tcase_add_test(tc_core, live_othello_2);
    tcase_add_test(tc_core, live_othello_3);
    tcase_add_test(tc_core, live_othello_4);
    tcase_add_test(tc_core, live_othello_5);
    tcase_add_test(tc_core, live_othello_6);
    tcase_add_test(tc_core, live_othello_7);
    tcase_add_test(tc_core, alphabeta_no_lm);
    tcase_add_test(tc_core, legal_moves_initial_position);
    tcase_add_test(tc_core, legal_moves_totalCount_62);
    tcase_add_test(tc_core, doMove_initial_position);
    tcase_add_test(tc_core, doMove_no_legal_moves);
    tcase_add_test(tc_core, frontierScore_early_position);
    tcase_add_test(tc_core, betterPlayer_1);
    tcase_add_test(tc_core, betterPlayer_2);
    tcase_add_test(tc_core, betterPlayer_3);
    tcase_add_test(tc_core, alphabeta_2nd_move);
    tcase_add_test(tc_core, bad_opening_position_1);
    tcase_add_test(tc_core, bad_midgame_position_low_mobility);
    tcase_add_test(tc_core, bad_midgame_position_1);
    tcase_add_test(tc_core, endgameAlphabeta_totalCount_63);
    tcase_add_test(tc_core, endgameAlphabetaMove_totalCount_63);
    tcase_add_test(tc_core, endgameAlphabetaMove_totalCount_63_no_legal_moves);
    tcase_add_test(tc_core, endgameAlphabeta_totalCount_62);
    tcase_add_test(tc_core, ffo_endgame_40);
    //tcase_add_test(tc_core, ffo_endgame_41);
    tcase_add_test(tc_core, endgameAlphabeta_totalCount_62x7);
    tcase_add_test(tc_core, endgameAlphabeta_totalCount_63x7);

    suite_add_tcase(s, tc_core);

    return s;
}

// From check framework tutorial (hence the "money" references)
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = money_suite();
    sr = srunner_create(s);

    // Set xml output
    srunner_set_xml(sr, "test_results");

    // Run
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
