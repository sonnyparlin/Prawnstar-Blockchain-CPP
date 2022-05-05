#ifndef _NODE_H_
#define _NODE_H_
#include <vector>
#include <string>
#include "TransactionPool.hpp"
#include "Wallet.hpp"
#include "Blockchain.hpp"
#include "Block.hpp"
#include "Message.hpp"
#include "AccountModel.hpp"
#include "SocketCommunication.hpp"
#include "ProofOfStake.hpp"
#include <iostream>
#include <fstream>
#include <mutex>

class SocketCommunication;
class Blockchain;
class Wallet;
class ProofOfStake;
class AccountModel;

/*!
 * Our primary interface between our peer to peer network and API.
 */
class Node {
private:
    static Node *node;

public:
    // constructors/destructors
    /*!
     *
     * @param argv
     *
     * Constructor for our main Node class (singleton)
     */
    explicit Node(char **argv); // Constructor
    ~Node();                      // Destructor

    // class methods
    static Node *createNode(char **argv);
    void startServers(int argc, char **argv);
    bool handleTransaction (Transaction&, bool broadcast=true);
    void handleBlock (Block &block, bool broadcast);
    void requestChain() const;
    void forge();
    void handleBlockchainRequest(std::string) const;
    void handleBlockchain(const std::string &blockchainString) const;
    std::string getNodeID() const;
    static std::string getConsoleLog();
    void log(std::string const& msg);

    // class members
    TransactionPool transactionPool; /*!< Node transaction pool */
    Wallet *nodeWallet; /*!< This node's wallet. */
    Wallet *exchangeWallet; /*!< Exchange wallet */
    Wallet *bobWallet; /*!< test wallet */
    Wallet *aliceWallet; /*!< test wallet */
    Wallet *node2Wallet; /*!< test wallet */
    Blockchain *blockchain; /*!< Node's blockchain */
    AccountModel *accountModel; /*!< Node's accountModel */
    SocketCommunication *p2p; /*!< Node's SocketCommunication */
    ProofOfStake *proofOfStake; /*!< Node's proofOfStake */
    // std::mutex nodeMutex; /*!< Mutex for */
    std::mutex logMutex; /*!< mutex for accessing shared log file */
};

#endif // _NODE_H_