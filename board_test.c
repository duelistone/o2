#include "board.h"
#include "search.h"
#include "assert.h"

// doMove time: 12.1-12.3 ns
// findLegalMove time: initial 14.1-14.3 ns, middle 15.4-15.6, end 11.7-11.9

int main(int argc, char ** argv) {
    // Declarations and memory
    u64 *black, *white;
    u8 *move;
    black = MU64(1);
    white = MU64(1);
    move = MU8(1);

    // Setup starting position
    *black = STARTING_BOARD_BLACK;
    *white = STARTING_BOARD_WHITE;

    // Perform test
    int counter = 0;
    while (counter < 55) {
        int ee, ee2;
        if (counter % 2 == 0) {
            *move = alphabetaMove(*black, *white, 1, -MAX_EVAL, MAX_EVAL);
            ee = (alphabetaMove(*black, *white, 1, -MAX_EVAL, MAX_EVAL) &~ *move) / 256;
            ee2 = alphabeta(*black, *white, 1, -MAX_EVAL, MAX_EVAL);
            *black = doMove(*black, *white, *move);
            *white &= ~*black;
            puts("black");
        }
        else {
            *move = alphabetaMove(*white, *black, 1, -MAX_EVAL, MAX_EVAL);
            ee = (alphabetaMove(*white, *black, 1, -MAX_EVAL, MAX_EVAL) &~ *move) / 256;
            ee2 = alphabeta(*white, *black, 1, -MAX_EVAL, MAX_EVAL);
            *white = doMove(*white, *black, *move);
            *black &= ~*white;
            puts("white");
        }

        printf("%d\n", counter);
        printBoard(*black, *white);
        printf("ab: %d\n", ee);
        printf("simple ab: %d\n", ee2);
        printf("abMove: %d\n", *move);
        printf("Eval (for board above): %d\n", eval(*black, *white));
        counter++;
    }
    endgameAlphabetaMove(*black, *white, -1, 1);
    
    // Print final position
    printBoard(*black, *white);
    printf("Eval (for board above): %d\n", eval(*black, *white));

    // Free memory
    free(move);
    free(black);
    free(white);

    return 0;
}
