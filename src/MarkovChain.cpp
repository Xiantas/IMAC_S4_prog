#include "MarkovChain.h"
#include <random>

namespace maths {

// Fonction utilitaire pour générer des nombres aléatoires entre 0 et 1
static double rand_0_1() {
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local std::uniform_real_distribution<double> distrib(0.0, 1.0);
    return distrib(gen);
}

MarkovChain::MarkovChain() : currentState(0) {
    // Définition des probabilités de transition
    transitions[0][0] = 0.1; transitions[0][1] = 0.9; transitions[0][2] = 0.0;
    transitions[1][0] = 0.0; transitions[1][1] = 0.8; transitions[1][2] = 0.2;
    transitions[2][0] = 0.1; transitions[2][1] = 0.0; transitions[2][2] = 0.9;
}

void MarkovChain::nextState() {
    double randValue = rand_0_1();
    double sum = 0;
    for (int i = 0; i < 3; ++i) {
        sum += transitions[currentState][i];
        if (randValue < sum) {
            currentState = i;
            break;
        }
    }
}

std::string MarkovChain::getCurrentStateName() const {
    switch (currentState) {
        case 0: return "Morning";
        case 1: return "Afternoon";
        case 2: return "Evening";
        default: return "Unknown";
    }
}

} // namespace maths
