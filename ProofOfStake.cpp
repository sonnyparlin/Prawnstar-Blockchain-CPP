#include "ProofOfStake.hpp"
#include "utils.hpp"
#include "Wallet.hpp"
#include <cstdint>
#include <cstdlib>
#include <climits>
#include <functional>

ProofOfStake::ProofOfStake(Node *node) {
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
    int stake;

    for ( const auto &validator : stakers ) {
        std::string validatorString = validator.first;

        if (getStake(validatorString) >= 100000)
            stake = 20;
        else if (getStake(validatorString) >= 50000)
            stake = 19;
        else if (getStake(validatorString) >= 25000)
            stake = 8;
        else if (getStake(validatorString) >= 10000)
            stake = 7;
        else if (getStake(validatorString) >= 5000)
            stake = 8;
        else if (getStake(validatorString) >= 1000)
            stake = 6;
        else if (getStake(validatorString) >= 500)
            stake = 5;
        else if (getStake(validatorString) >= 100)
            stake = 4;
        else if (getStake(validatorString) >= 50)
            stake = 3;
        else if (getStake(validatorString) >= 10)
            stake = 2;
        else if (getStake(validatorString) >= 1)
            stake = 1;

        Lot l(validatorString, stake, seed);
        lots.push_back(l);
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
    // srand (hashed);
    int referenceHashIntValue = fnv1a(std::to_string(hashed)) % INT_MAX;
    // std::cout << "referenceHashIntValue: " << referenceHashIntValue << std::endl;
    for (auto lot : lots) {
        int lotIntValue = fnv1a(lot.lotHash()) % INT_MAX;
        // std::cout << "lotIntValue: " << lotIntValue << std::endl;
        int offset = abs(int(lotIntValue - referenceHashIntValue));
        // std::cout << "offset: " << offset << std::endl;
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
