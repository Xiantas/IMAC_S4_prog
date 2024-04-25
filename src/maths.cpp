#include <iostream>
#include <random>

#define M_PI 3.14159265358979323846

namespace maths {
    double rand_0_1() {
        thread_local std::default_random_engine gen{std::random_device{}()};
        thread_local auto distrib =
        std::uniform_real_distribution<long double>{0.000000000, 1.000000000};
        return distrib(gen);
    }

    bool bernoulli(double p) {
        return rand_0_1() < p;
    }

    double cauchyRandom(double location, double scale) {
        double u = rand_0_1();
        return location + scale * tan(M_PI * (u - 0.5));
    }
    
    int sgn(double v) {
        return (v < 0) ? -1 : (v > 0) ? 1 : 0;
    }
    double laplaceRandom(double mu, double b) {
        double u = rand_0_1() - 0.5;
        return mu - b * sgn(u) * log(1 - 2 * fabs(u));
    }
    std::string determineDaytime(double mu, double b) {
        double hour = laplaceRandom(mu, b);
        if (hour < 12) return "morning";
        else if (hour < 18) return "afternoon";
        else return "evening";
    }

    int poissonRandom(double lambda) {
        double L = exp(-lambda);
        int k = 0;
        double p = 1;
        while (p > L) {
            k++;
            double u = rand_0_1();
            p *= u;
        }
        return k - 1;
    }

    double exponentialRandom(double theta) {
        double U = rand_0_1();
        return -theta * std::log(U);
    }

    // Utiliser pour taille tortue (fonctionne que pour alpha entier et alpha = petite valeur)
    double gammaRandom(int alpha, double theta) {
        double sum = 0.0;
        for (int i = 0; i < alpha; i++) {
            sum += exponentialRandom(theta);
        }
        return sum;
    }

    
}   // namespace maths
