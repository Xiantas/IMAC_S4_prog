// maths.h
#ifndef MATHS_H
#define MATHS_H

namespace maths {
    double rand_0_1();
    bool bernoulli(double p);
    double exponentialRandom(double theta);
    double gammaRandom(int alpha, double theta);
    double cauchyRandom(double location, double scale);
    int poissonRandom(double lambda);
    
}

#endif // MATHS_H
