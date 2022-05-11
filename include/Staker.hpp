#ifndef PRAWNSTARBLOCKCHAIN_STAKER_H
#define PRAWNSTARBLOCKCHAIN_STAKER_H
#include <string>

class Staker {
public:
    Staker(std::string&, std::string&, double&);
    ~Staker();

    double getStake(std::string&);
    double updateStake(std::string&, double &&);

    std::string stakerPublicKey {};
    std::string stakerType {}; // PHYSICAL, VIRTUAL
    double stakerStake {};
};

#endif //PRAWNSTARBLOCKCHAIN_STAKER_H
