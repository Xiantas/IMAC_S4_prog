// maths.h
#ifndef MATHS_H
#define MATHS_H

namespace maths {
    double rand_0_1();
    bool bernoulli(double p);
    double exponentialRandom(double theta);
    double gammaRandom(int alpha, double theta);
}

#endif // MATHS_H
