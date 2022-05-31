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
 * The node is the interface for both the API and the P2P network to interact with the
 * blockchain. The node is where transactions and blocks are handled whether they come
 * from the API or the P2P network.
 */
class Node {
private:
    static Node *node;

public:
    /*!
     *
     * @param argv
     *
     * Constructor for our main Node class (singleton)
     */
    explicit Node(char **argv); // Constructor
    ~Node();                    // Destructor

    // class methods
    static Node *createNode(char **argv);
    void startServers(int argc, char **argv);
    bool handleTransaction (Transaction&, bool broadcast=true);
    void handleBlock (Block &block, bool broadcast);
    void requestChain() const;
    static std::string getTimeStr();
    void forge();
    void handleBlockchainRequest(std::string) const;
    void handleBlockchain(const std::string &blockchainString) const;
    std::string getNodeID() const;
    static std::string getConsoleLog();
    void log(std::string const& msg);

    // class members
    Wallet *nodeWallet {}; /*!< This node's wallet. */
    Wallet *exchangeWallet {}; /*!< Exchange wallet */
    Wallet *bobWallet {}; /*!< test wallet */
    Wallet *aliceWallet {}; /*!< test wallet */
    Wallet *node2Wallet {}; /*!< test wallet */
    Blockchain *blockchain {}; /*!< Node's blockchain */
    AccountModel *accountModel {}; /*!< Node's accountModel */
    SocketCommunication *p2p {}; /*!< Node's SocketCommunication */
    ProofOfStake *proofOfStake {}; /*!< Node's proofOfStake */
    std::mutex logMutex {}; /*!< mutex for accessing shared log file */
};

#endif // _NODE_H_