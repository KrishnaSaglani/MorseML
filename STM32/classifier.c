#include <math.h>
#include "classifier.h"
#include "model_params.h"

/* Gaussian probability */
float gaussian(float x, float mean, float var) {
    float diff = x - mean;
    return -(diff * diff) / (2 * var);
}

char predict(float *input) {

    float best_score = -1e9;
    int best_class = 0;

    for (int c = 0; c < 5; c++) {

        float score = 0;

        for (int i = 0; i < 5; i++) {
            score += gaussian(input[i], means[c][i], vars[c][i]);
        }

        if (score > best_score) {
            best_score = score;
            best_class = c;
        }
    }

    return labels[best_class];
}