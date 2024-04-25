#ifndef MARKOVCHAIN_H
#define MARKOVCHAIN_H

#include <string>

namespace maths {

class MarkovChain {
public:
    MarkovChain();
    void nextState();
    std::string getCurrentStateName() const;

private:
    double transitions[3][3];
    int currentState;
};

} // namespace maths

#endif // MARKOVCHAIN_H