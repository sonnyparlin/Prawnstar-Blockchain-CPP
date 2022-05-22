#ifndef _PROOFOFSTAKE_H_
#define _PROOFOFSTAKE_H_
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <cmath>
#include "Node.hpp"
#include <mutex>

//class Node;

/*!
 * Proof of Stake, our blockchain's consensus algorithm.
 */
class ProofOfStake {
private:
    Node *node;
public:
    explicit ProofOfStake(Node *node);
    ~ProofOfStake();

    void update(const std::string&, double stake);
    double getStake(const std::string&);
    inline std::vector<std::string> validatorLots(const std::string& seed);
    static std::string winnerLot(const std::vector<std::string>&, const std::string &);
    std::string forger(const std::string&);
    void setGenesisNodeStake(const std::string&);
    inline static uint32_t fnv1a_hash(const std::string& text);
    inline static uint32_t lcg_rand(uint32_t *state);

    std::unordered_map<std::string, double>stakers {}; /*!< list of stakers */
    std::mutex posMutex {}; /*!< mutext for guarding shared data (stakers list) */
    std::string genesisNodeStake {}; /*!< variable used to compare this node's
                                      * public key to the master node wallet's public key */
};

#endif // _PROOFOFSTAKE_H_
