#include "ProofOfStake.hpp"
#include "utils.hpp"
#include "Wallet.hpp"
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <functional>

ProofOfStake::ProofOfStake(Node *node, int port) {
    this->node = node;

    Wallet genesisNodeWallet(node);
    genesisNodeWallet.fromKey("genesisNode.pem");
    node->accountModel->addAccount(genesisNodeWallet.address, 
                                genesisNodeWallet.walletPublicKey, 
                                genesisNodeWallet.walletPrivateKey);
    setGenesisNodeStake(genesisNodeWallet.walletPublicKey);
}

ProofOfStake::~ProofOfStake() {
}

void ProofOfStake::setGenesisNodeStake(std::string genesisPublicKey) {
    stakers[genesisPublicKey] = 1;
}

void ProofOfStake::update(std::string publicKeyString, double stake) {
    std::lock_guard<std::mutex> guard(posMutex);
    if (stakers.find(publicKeyString) != stakers.end())
        stakers[publicKeyString] += stake;
    else
        stakers[publicKeyString] = stake;
}

double ProofOfStake::getStake(std::string publicKeyString) {
    std::lock_guard<std::mutex> guard(posMutex);
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
    // std::string hash = utils::hash(seed);
    std::hash<std::string> hasher;
    auto hashed = hasher(seed);
    
    std::cout << "hashInt: " << hashed << std::endl;

    srand (999);
    int referenceHashIntValue = rand() % (INT_MAX - 1) + 1;

    for (auto lot : lots) {
        int lotIntValue = rand() % (INT_MAX - 1) + 1;
        int offset = abs(int(lotIntValue - referenceHashIntValue));
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
