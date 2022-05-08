#include "ProofOfStake.hpp"
#include "utils.hpp"
#include "Wallet.hpp"
#include <cstdint>

/*!
 *
 * @param passedInNode
 *
 * Constructor for the ProofOfStake class, starts genesis node
 * sets up this node's wallet as a staker so the system will run.
 */
ProofOfStake::ProofOfStake(Node *passedInNode)
    : node(passedInNode) {

    Wallet genesisNodeWallet(node);
    genesisNodeWallet.fromKey("genesisNode.pem");
    node->accountModel->addAccount(genesisNodeWallet.address, 
                                genesisNodeWallet.walletPublicKey);
    setGenesisNodeStake(genesisNodeWallet.walletPublicKey);
}

ProofOfStake::~ProofOfStake()=default;

/*!
 *
 * @param genesisPublicKey
 *
 * Set up the genesis node stake so the system will run before real stakers
 * show up.
 */
void ProofOfStake::setGenesisNodeStake(const std::string &genesisPublicKey) {
    stakers[genesisPublicKey] = 1;
}

/*!
 *
 * @param publicKeyString
 * @param stake
 *
 * Update the stake of the user with publicKeyString to the new amount specified
 * in stake.
 */
void ProofOfStake::update(const std::string &publicKeyString, double stake) {
    std::lock_guard<std::mutex> guard(posMutex);
    if (stakers.find(publicKeyString) != stakers.end())
        stakers[publicKeyString] += stake;
    else
        stakers[publicKeyString] = stake;
}

/*!
 *
 * @param publicKeyString
 * @return double
 *
 * Get the stake of the user with publicKeyString
 */
double ProofOfStake::getStake(const std::string &publicKeyString) {
    std::lock_guard<std::mutex> guard(posMutex);
    if (stakers.find(publicKeyString) != stakers.end())
        return stakers[publicKeyString];
    else
        return 0;
}

/*!
 *
 * @param seed
 * @return std::vector<Lot>
 *
 * Create lottery tickets for the stakers to be chosen at random
 * to decide who the next forger is. The number of lottery tickets
 * for each staker corresponds to the amount of tokens they are staking.
 */
inline std::vector<Lot> ProofOfStake::validatorLots(const std::string& seed) {
    std::vector<Lot> lots;

    for ( const auto &validator : stakers ) {

        /*!
         * Note, it doesn't do you any good to stake
         * more than upperLimit tokens. (limit is for performance).
         */
        int upperLimit = 500; // how many loops do you want to allow?
        auto stake = static_cast<int>(validator.second);

        // Without this guard our network would get overloaded.
        if (stake > upperLimit)
            stake = upperLimit;

        for (int i = 0; i < stake; i++) {
            Lot l(validator.first);
            lots.push_back(l);
        }
    }
    return lots;
}

/*!
 *
 * @param text
 * @return uint32_t
 *
 * Hashing function that is platform independant.
 */
inline uint32_t ProofOfStake::fnv1a(const std::string& text) {
     // 32 bit params
     uint32_t constexpr fnv_prime = 16777619U;
     uint32_t constexpr fnv_offset_basis = 2166136261U;

    // 64 bit params
    // uint64_t constexpr fnv_prime = 1099511628211ULL;
    // uint64_t constexpr fnv_offset_basis = 14695981039346656037ULL;

    uint64_t hash = fnv_offset_basis;
    
    for(const auto& c: text) {
        hash ^= c;
        hash *= fnv_prime;
    }

    return hash;
}

/*!
 *
 * @param state
 * @return uint32_t
 *
 * A linear congruential generator (LCG) is an algorithm that
 * yields a sequence of pseudo-randomized numbers calculated
 * with a discontinuous piecewise linear equation. The method
 * represents one of the oldest and best-known pseudorandom
 * number generator algorithms.
 */
inline uint32_t ProofOfStake::lcg_rand(uint32_t *state)
{
    return *state = (uint64_t)*state * 279470273u % 0xfffffffb;
}

/*!
 *
 * @param lots
 * @param seed
 * @return Lot
 *
 * Chooses a winner for the forger lottery based on the random selection of
 * Lots.
 */
inline Lot ProofOfStake::winnerLot(const std::vector<Lot> &lots, const std::string &seed) {
    auto hashed = fnv1a(seed);
    const auto &winnerLot = lots.at(lcg_rand(&hashed) % lots.size());
    return winnerLot;
}

/*!
 *
 * @param lastBlockHash
 * @return std::string
 *
 * forger gets a list of all the validator Lots and selects
 * one at random. The number of validator lots corresponds to
 * the amount of tokens that are staked by a staker/validator.
 */
std::string ProofOfStake::forger(const std::string &lastBlockHash) {
    std::vector<Lot> lots = validatorLots(lastBlockHash);
    auto winner = winnerLot(lots, lastBlockHash);
    return winner.publicKeyString;
}
