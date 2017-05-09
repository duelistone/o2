// main.c

// Executable for othello engine following Caltech's CS2 protocol

#include "search.h"
#include <stdio.h>
#include <stdlib.h>

#define ENDGAME_DEPTH 41

int main(int argc, char **argv) {
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

        if (x != -1) {
            // Casework based on player to move
            if (sideToMove) {
                white = doMove(white, black, SQUARE(x, y));
                black &= ~white;
                if (TC(black, white) >= ENDGAME_DEPTH) move = endgameAlphabetaMove(black, white, -1, 1) & 0xFF;
                else move = alphabetaMove(black, white, 6, -MAX_EVAL, MAX_EVAL) & 0xFF;
                if (move != NULL_MOVE) {
                    black = doMove(black, white, move);
                    white &= ~black;
                }
            }
            else {
                black = doMove(black, white, SQUARE(x, y));
                white &= ~black;
                if (TC(black, white) >= ENDGAME_DEPTH) move = endgameAlphabetaMove(white, black, -1, 1) & 0xFF;
                else move = alphabetaMove(white, black, 6, -MAX_EVAL, MAX_EVAL) & 0xFF;
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
                if (TC(black, white) >= ENDGAME_DEPTH) move = endgameAlphabetaMove(black, white, -1, 1) & 0xFF;
                else move = alphabetaMove(black, white, 6, -MAX_EVAL, MAX_EVAL) & 0xFF;
                if (move != NULL_MOVE) {
                    black = doMove(black, white, move);
                    white &= ~black;
                }
            }
            else {
                if (TC(black, white) >= ENDGAME_DEPTH) move = endgameAlphabetaMove(white, black, -1, 1) & 0xFF;
                else move = alphabetaMove(white, black, 6, -MAX_EVAL, MAX_EVAL) & 0xFF;
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
        fprintf(stderr, "eval %d\n", eval(black, white));

        // Make sure cout has been flushed
        fflush(stdout);

        // Increment loop counter
        counter++;
    }

    return 0;
}
