#include "ProofOfStake.hpp"

ProofOfStake::ProofOfStake() {
}

ProofOfStake::~ProofOfStake() {
}

void ProofOfStake::update(std::string publicKeyString, double stake) {
    if (stakers.find(publicKeyString) != stakers.end())
        stakers[publicKeyString] += stake;
    else
        stakers[publicKeyString] = stake;
}

double ProofOfStake::getStake(std::string publicKeyString) {
    if (stakers.find(publicKeyString) != stakers.end())
        return stakers[publicKeyString];
    else
        return 0;
}