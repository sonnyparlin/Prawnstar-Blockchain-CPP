#ifndef _PROOFOFSTAKE_H_
#define _PROOFOFSTAKE_H_
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <cmath>
#include "Lot.hpp"
#include "Node.hpp"
#include <mutex>

class Node;

class ProofOfStake {
private:
    Node *node;
public:
    std::unordered_map<std::string, double>stakers;
    std::mutex posMutex;

    explicit ProofOfStake(Node *node);           // Constructor
    ~ProofOfStake();          // Destructor

    void update(const std::string&, double stake);
    double getStake(const std::string&);
    inline std::vector<Lot> validatorLots(const std::string& seed);
    static Lot winnerLot(const std::vector<Lot>&, const std::string &);
    std::string forger(const std::string&);
    void setGenesisNodeStake(const std::string&);
    inline static uint32_t fnv1a(const std::string& text);
    inline static uint32_t lcg_rand(uint32_t *state);
};

#endif // _PROOFOFSTAKE_H_
