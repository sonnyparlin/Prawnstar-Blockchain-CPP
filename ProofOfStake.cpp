#include "ProofOfStake.hpp"
#include "utils.hpp"
#include "Wallet.hpp"
#include <cstdint>
#include <functional>

ProofOfStake::ProofOfStake(Node *passedInNode) {
    node = passedInNode;

    Wallet genesisNodeWallet(node);
    genesisNodeWallet.fromKey("genesisNode.pem");
    node->accountModel->addAccount(genesisNodeWallet.address, 
                                genesisNodeWallet.walletPublicKey, 
                                genesisNodeWallet.walletPrivateKey);
    setGenesisNodeStake(genesisNodeWallet.walletPublicKey);
}

ProofOfStake::~ProofOfStake()=default;

void ProofOfStake::setGenesisNodeStake(const std::string &genesisPublicKey) {
    stakers[genesisPublicKey] = 1;
}

void ProofOfStake::update(const std::string &publicKeyString, double stake) {
    std::lock_guard<std::mutex> guard(posMutex);
    if (stakers.find(publicKeyString) != stakers.end())
        stakers[publicKeyString] += stake;
    else
        stakers[publicKeyString] = stake;
}

double ProofOfStake::getStake(const std::string &publicKeyString) {
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

        if (getStake(validatorString) >= 1000)
            stake = 5;
        else if (getStake(validatorString) >= 500)
            stake = 4;
        else if (getStake(validatorString) >= 100)
            stake = 3;
        else if (getStake(validatorString) >= 50)
            stake = 2;
        else if (getStake(validatorString) >= 1)
            stake = 1;

        for (int i = 0; i < stake; i++) {
            Lot l(validatorString, stake, seed);
            lots.push_back(l);
        }
    }
    return lots;
}

inline uint32_t fnv1a(std::string const & text) {
     // 32 bit params
     uint32_t constexpr fnv_prime = 16777619U;
     uint32_t constexpr fnv_offset_basis = 2166136261U;

    // 64 bit params
//    uint64_t constexpr fnv_prime = 1099511628211ULL;
//    uint64_t constexpr fnv_offset_basis = 14695981039346656037ULL;
//
    uint64_t hash = fnv_offset_basis;
    
    for(auto c: text) {
        hash ^= c;
        hash *= fnv_prime;
    }

    return hash;
}

uint32_t lcg_rand(uint32_t *state)
{
    return *state = (uint64_t)*state * 279470273u % 0xfffffffb;
}

Lot ProofOfStake::winnerLot(const std::vector<Lot> &lots, const std::string &seed) {
    Lot winnerLot;
    // int leastOffset = {0};
    // std::string hash = utils::hash(seed);
    auto hashed = fnv1a(seed);

//    int referenceHashIntValue = fnv1a(std::to_string(hashed)) % INT_MAX;
//    // std::cout << "referenceHashIntValue: " << referenceHashIntValue << std::endl;
//    for (auto lot : lots) {
//        int lotIntValue = fnv1a(lot.lotHash()) % INT_MAX;
//        // std::cout << "lotIntValue: " << lotIntValue << std::endl;
//        int offset = abs(int(lotIntValue - referenceHashIntValue));
//        // std::cout << "offset: " << offset << std::endl;
//        if (leastOffset == 0 || offset < leastOffset) {
//            leastOffset = offset;
//            winnerLot = lot;
//        }
//    }
    // srand(hashed);
    winnerLot = lots.at(lcg_rand(&hashed) % lots.size());
    return winnerLot;
}

std::string ProofOfStake::forger(const std::string &lastBlockHash) {
    std::vector<Lot> lots = validatorLots(lastBlockHash);
    Lot winner;
    winner = winnerLot(lots, lastBlockHash);
    return winner.publicKeyString;
}
