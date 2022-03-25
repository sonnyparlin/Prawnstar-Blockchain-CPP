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

inline uint64_t fnv1a(std::string const & text) {
    // 32 bit params
    // uint32_t constexpr fnv_prime = 16777619U;
    // uint32_t constexpr fnv_offset_basis = 2166136261U;

    // 64 bit params
    uint64_t constexpr fnv_prime = 1099511628211ULL;
    uint64_t constexpr fnv_offset_basis = 14695981039346656037ULL;
    
    uint64_t hash = fnv_offset_basis;
    
    for(auto c: text) {
        hash ^= c;
        hash *= fnv_prime;
    }

    return hash;
}

Lot ProofOfStake::winnerLot(std::vector<Lot> lots, std::string seed) {
    Lot winnerLot;
    int leastOffset = {0};
    // std::string hash = utils::hash(seed);
    auto hashed = fnv1a(seed);
    
    // std::cout << "hashInt: " << hashed << std::endl;
    srand (hashed);
    int referenceHashIntValue = rand() % INT_MAX;

    for (auto lot : lots) {
        int lotIntValue = rand() % INT_MAX;
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
