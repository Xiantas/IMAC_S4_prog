#include "maths.h"
#include <cstdlib>  
#include <ctime>   

namespace maths {

    float randomUniformFloat(float min, float max) {
        float scale = rand() / (float) RAND_MAX; 
        return min + scale * (max - min); 
    }

    bool bernoulli(double p) {
        return rand() / (float) RAND_MAX < p;
    }
}
