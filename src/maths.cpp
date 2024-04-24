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

    //new:
    double cauchyRandom(double location, double scale) {
        double u = rand_0_1();
        return location + scale * tan(M_PI * (u - 0.5));
    }
    
    double normalRandom(double mean, double stddev) {
        double u1 = rand_0_1();
        double u2 = rand_0_1();
        double z0 = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
        return z0 * stddev + mean;
    }

    int poissonRandom(double lambda) {
        double L = exp(-lambda);
        int k = 0;
        double p = 1;
        do {
            k++;
            double u = rand_0_1();
            p *= u;
        } while (p > L);
        return k - 1;
    }

    int binomialRandom(int n, double p) {
        int x = 0;
        for (int i = 0; i < n; i++) {
            if (rand_0_1() < p) x++;
        }
        return x;
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
    //intégration de la fonction gamma pour obtenir la taille de la tortue
    // int main() {
    //     int alpha = 3;  // Paramètre de forme, entier
    //     double theta = 0.5;  // Paramètre d'échelle

    //     double gammaSize = gammaRandom(alpha, theta);
    //     std::cout << "Generated Gamma Size: " << gammaSize << std::endl;

    //     // Autres parties de votre simulation
    //     return 0;
    // }


}   // namespace maths
