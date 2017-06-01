// main.c

// Executable for othello engine following Caltech's CS2 protocol

#include "search.h"
#include <stdio.h>
#include <stdlib.h>

#define AB_DEPTH 12

int main(int argc, char **argv) {
    // Initialize transposition table
    initializeTT();

    // Setup board and move pointers
    u64 black, white;
    black = STARTING_BOARD_BLACK;
    white = STARTING_BOARD_WHITE;

    u8 move;

    int x, y, secondsLeft;
    int counter = 0;
    int sideToMove = WHITE;

    // Notify readiness
    puts("Init done");
    fflush(stdout);

    // Scan from input line
    while (1) {
        int numRead = scanf("%d %d %d", &x, &y, &secondsLeft);
        if (numRead < 3) {
            fprintf(stderr, "scanf error");
            continue;
        }

        int result; // For result of alphabetaMove or endgameAlphabetaMove call

        if (x != -1) {
            // Casework based on player to move
            if (sideToMove) {
                white = doMove(white, black, SQUARE(x, y));
                black &= ~white;
                AB_SCOUT_STDERR(black, white);
                if (TC(black, white) >= ENDGAME_START) {
                    result = endgameAlphabetaMove(black, white, 0, 1);
                    if (result == 0) result = endgameAlphabetaMove(black, white, -1, 0);
                }
                else result = alphabetaMove(black, white, AB_DEPTH, -MAX_EVAL, MAX_EVAL);
                move = EXTRACT_MOVE(result);
                if (move != NULL_MOVE) {
                    black = doMove(black, white, move);
                    white &= ~black;
                }
            }
            else {
                black = doMove(black, white, SQUARE(x, y));
                white &= ~black;
                AB_SCOUT_STDERR(white, black);
                if (TC(black, white) >= ENDGAME_START) {
                    result = endgameAlphabetaMove(white, black, 0, 1);
                    if (result == 0) result = endgameAlphabetaMove(white, black, -1, 0);
                }
                else result = alphabetaMove(white, black, AB_DEPTH, -MAX_EVAL, MAX_EVAL);
                move = EXTRACT_MOVE(result);
                if (move != NULL_MOVE) {
                    white = doMove(white, black, move);
                    black &= ~white;
                }
            }

            // Print and make move
            if (move != NULL_MOVE) {
                printf("%d %d\n", move % 8, move / 8);
            }
            else {
                puts("-1 -1");
            }
        }
        else {
            // Set correct side to move at first
            if (counter == 0) sideToMove = BLACK;

            // Casework based on player to move
            if (sideToMove) {
                AB_SCOUT_STDERR(black, white);
                if (TC(black, white) >= ENDGAME_START) {
                    result = endgameAlphabetaMove(black, white, 0, 1);
                    if (result == 0) result = endgameAlphabetaMove(black, white, -1, 0);
                }
                else result = alphabetaMove(black, white, AB_DEPTH, -MAX_EVAL, MAX_EVAL);
                move = EXTRACT_MOVE(result);
                if (move != NULL_MOVE) {
                    black = doMove(black, white, move);
                    white &= ~black;
                }
            }
            else {
                AB_SCOUT_STDERR(white, black);
                if (TC(black, white) >= ENDGAME_START) {
                    result = endgameAlphabetaMove(white, black, 0, 1);
                    if (result == 0) result = endgameAlphabetaMove(white, black, -1, 0);
                }
                else result = alphabetaMove(white, black, AB_DEPTH, -MAX_EVAL, MAX_EVAL);
                move = EXTRACT_MOVE(result);
                if (move != NULL_MOVE) {
                    white = doMove(white, black, move);
                    black &= ~white;
                }
            }

            // Print move
            if (move != NULL_MOVE) {
                printf("%d %d\n", move % 8, move / 8);
            }
            else {
                puts("-1 -1");
            }
        }            

        // Logging
        printBoard2(black, white);
        if (findLegalMoves(black, white) | findLegalMoves(white, black)) {
            fprintf(stderr, "last move %u\n", move);
            if (!sideToMove) printEval2(black, white);
            else printEval2(white, black);
            fprintf(stderr, "ab %d\n", EXTRACT_EVAL(result));
        }
        else fprintf(stderr, "final result %d\n", DD(black, white));

        // Make sure cout has been flushed
        fflush(stdout);

        // Increment loop counter
        counter++;
    }

    return 0;
}
