#include "search.h"
#include <check.h>

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

START_TEST(endgameAlphabeta_totalCount_63)
{
    u64 black = -1ull & ~(BIT(63) | BIT(0));
    u64 white = BIT(63);
    ck_assert_int_gt(endgameAlphabeta(black, white, -1, 1), 0);
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
    ck_assert_int_gt(endgameAlphabeta(black, white, -1, 1), 0);
}
END_TEST

// Best time: 0.003786
START_TEST(ffo_endgame_40)
{
    u64 black = C3 | D3 | C4 | H1 | H2 | H3 | H4 | H5 | H6 | H7 | G4 | G5;
    u64 white = A1 | A3 | A4 | A5 | B2 | B3 | B4 | B5 | C2 | C5 | D1 | D2
              | D4 | D5 | D6 | E1 | E2 | E3 | E4 | E5 | E6 | E7 | F1 | F2
              | F3 | F4 | F5 | F6 | G1 | G2 | G3 | G6;
    ck_assert_int_gt(endgameAlphabeta(black, white, -1, 1), 0);
}
END_TEST

// Best time: 26 sec
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

Suite * money_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Othello");
    tc_core = tcase_create("Core");
    
    // Give longer timeout
    tcase_set_timeout(tc_core, 30);

    // Add tests here by name
    tcase_add_test(tc_core, legal_moves_initial_position);
    tcase_add_test(tc_core, legal_moves_totalCount_62);
    tcase_add_test(tc_core, doMove_initial_position);
    tcase_add_test(tc_core, doMove_no_legal_moves);
    tcase_add_test(tc_core, endgameAlphabeta_totalCount_63);
    tcase_add_test(tc_core, endgameAlphabetaMove_totalCount_63);
    tcase_add_test(tc_core, endgameAlphabetaMove_totalCount_63_no_legal_moves);
    tcase_add_test(tc_core, endgameAlphabeta_totalCount_62);
    tcase_add_test(tc_core, ffo_endgame_40);
    tcase_add_test(tc_core, ffo_endgame_41);

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
