#include "nn.h"

void initializeNN() {
    for (int i = 5; i < 64; i++) {
        char file_name[30];
        int len = sprintf(file_name, "best_nn_%d_5_layer", i);
        if (access(file_name, F_OK) != -1) {
            anns[i] = fann_create_from_file(file_name);
        }
        else {
            anns[i] = NULL;
        }
    }
}

float nn_eval(u64 black, u64 white) {
    // Form input list
    fann_type inputList[64 * 3];
    BOARD_TO_INPUT_LIST(black, white, inputList);

    // Use correct fann and run
    assert(anns[TC(black, white)] != NULL);
    fann_type * output = fann_run(anns[TC(black, white)], inputList);

    return output[0];
}
