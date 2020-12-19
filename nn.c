#include "nn.h"

void initializeNN() {
    for (int i = 5; i < 64; i++) {
        char file_name[30];
        sprintf(file_name, "best_nn_%d_5_layer", i);
        if (access(file_name, F_OK) != -1) {
            // TODO
        }
        else {
            anns[i] = NULL;
        }
    }
}

float nn_eval(u64 black, u64 white) {
    // TODO
    return 0;
}
