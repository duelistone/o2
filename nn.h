#ifndef __NN_H__
#define __NN_H__

#include "defs.h"
#include <fann.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>

// List of neural networks, indexed by the total (disc) count they correspond to
struct fann * anns[64];

// Function that initializes the anns array
void initializeNN();

// Evaluation function using neural net
// Make sure to only use this function for a total (disc) count which a neural net
// exists for.
float nn_eval(u64 black, u64 white);

#endif
