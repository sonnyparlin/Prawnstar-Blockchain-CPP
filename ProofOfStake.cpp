#include "ProofOfStake.hpp"
#include "utils.hpp"
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

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

std::vector<Lot> ProofOfStake::validatorLots(std::string seed) {
    std::vector<Lot> lots;

    for ( const auto &validator : stakers ) {
        std::string validatorString = validator.first;
        for ( int i = 0; i < getStake(validatorString); i++ ) {
            int stake = getStake(validatorString);
            
            Lot l(validatorString, stake+1, seed);
            lots.push_back(l);
        }
    }
    return lots;
}

Lot ProofOfStake::winnerLot(std::vector<Lot> lots, std::string seed) {
    Lot winnerLot;
    int leastOffset = {0};
    std::string hash = utils::hash(seed);
    hash.resize(5);
    int hashInt = stoi(hash, 0, 16);
    
    // std::cout << "hashInt: " << hashInt << std::endl;

    srand (hashInt);
    int referenceHashIntValue = rand() % (INT_MAX - 1) + 1;

    for (auto lot : lots) {
        std::string tempString = lot.lotHash();
        int lotIntValue = rand() % (INT_MAX - 1) + 1;
        int offset = abs(int(lotIntValue - referenceHashIntValue));

        // std::cout << "lot_int_value " << lotIntValue << std::endl;
        // std::cout << "offset        " << offset << std::endl;

        if (leastOffset == 0 || offset < leastOffset) {
            leastOffset = offset;
            winnerLot = lot;
        }
    }
    return winnerLot;
}

std::string ProofOfStake::forger(std::string lastBlockHash) {
    std::vector<Lot> lots = validatorLots(lastBlockHash);
    Lot winner;
    winner = winnerLot(lots, lastBlockHash);
    return winner.publicKeyString;
}
