#include "Node.hpp"
#include "NodeApi.hpp"
#include <crow.h>
#include <chrono>
#include <thread>

Node * Node::node=nullptr;

Node::Node(int argc, char **argv) {
    p2p = new SocketCommunication(this);
    accountModel = new AccountModel(this);
    blockchain = new Blockchain(this);

    int port = utils::getPort(argv[2]);
    std::cout << "port is: " << port << std::endl;

    if (port == 10001)
        nodeWallet = new Wallet(this, true, "genesisNode.pem");
    else if (port == 10002)
        nodeWallet = new Wallet(this, true, "node2Wallet.pem");
    else
        nodeWallet = new Wallet(this);
    
    node2Wallet = new Wallet(this, true, "node2Wallet.pem");
    exchangeWallet = new Wallet(this, true, "exchange.pem");
    aliceWallet = new Wallet(this, true, "alice.pem");
    bobWallet = new Wallet(this, true, "bob.pem"); 

    std::cout << "walletAddress: " << nodeWallet->address << std::endl;
    std::cout << "exchangeAddress: " << exchangeWallet->address << std::endl;
    std::cout << "aliceAddress: " << aliceWallet->address << std::endl;
    std::cout << "bobAddress: " << bobWallet->address << std::endl;

    proofOfStake = new ProofOfStake(this, port);
}

Node::~Node() {
    delete p2p;
    delete blockchain;
    delete nodeWallet;
    delete proofOfStake;
    delete accountModel;
    delete exchangeWallet;
    delete node2Wallet;
    delete aliceWallet;
    delete bobWallet;
}

Node *Node::createNode(int argc, char **argv) {
    if (node == nullptr) {
        node = new Node(argc, argv);
    }

    return node;
}

void Node::startServers(int argc, char **argv) {
    p2p->startSocketCommunication(argc, argv);
    
    NodeApi api(this);
    if (argc == 4) {
        std::string po = std::string(argv[3]);
        api.start(po);
    } else if (argc == 5) {
        std::string po = std::string(argv[4]);
        api.start(po);
    }
}

/*!
This is where we handle transactions. Called from NodeAPI and SocketCommunication.
*/
bool Node::handleTransaction (Transaction transaction, bool broadcast ) {
    std::string data = transaction.payload();
    std::string signature = transaction.signature;
    std::string signerPublicKey = transaction.senderPublicKey;

    bool signatureValid = utils::verifySignature(data, signature, signerPublicKey);
    bool transactionExists = transactionPool.transactionExists(transaction);
    bool transactionInBlockChain = blockchain->transactionExists(transaction);
    bool transactionCovered = false;
    
    if (transaction.type != "EXCHANGE") {
        if (accountModel->getBalance(transaction.senderAddress) > transaction.amount)
            transactionCovered = true;
    } else
        transactionCovered = true;

    if (!transactionExists && !transactionInBlockChain && signatureValid && transactionCovered) {
        transactionPool.addTransaction(transaction);
        if (broadcast) {
            // std::cout << "broadcasting tx: " << transaction.toJson() << std::endl;
            Message message("TRANSACTION", transaction.toJson());
            std::string msgJson = message.toJson();
            p2p->broadcast(msgJson.c_str());
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        bool forgingRequired = transactionPool.forgerRequired();
        if (forgingRequired)
            forge();

        return true;
    }
    std::cout << "Transaction failed:" << std::endl;
    std::cout << "signatureValid: " << boolalpha << signatureValid << std::endl;
    std::cout << "transactionExists: " << boolalpha << transactionExists << std::endl;
    std::cout << "transactionCovered: " << boolalpha << transactionCovered << std::endl;
    std::cout << "transactionInBlockChain: " << boolalpha << transactionInBlockChain << std::endl;
    std::cout << transaction.toJson() << std::endl;

    return false;
}

/*
Do checks to make sure the block is valid before adding it to the chain.
Broadcast if necessary.
*/
void Node::handleBlock (Block block, bool broadcast) {
    Wallet forger(block.forgerAddress.c_str(), this);
    std::string blockHash = block.hash;
    bool blockCountValid = blockchain->blockCountValid(block);

    // Requesting node is not up to date, so let's give them the blocks they're missing.
    if (!blockCountValid) {
        requestChain();
        return;     
    }

    bool lastBlockHashValid = blockchain->lastBlockHashValid(block);
    bool forgerValid = blockchain->forgerValid(block);
    bool transactionValid = blockchain->transactionValid(block.transactions);
    bool signatureValid = utils::verifySignature(block.payload(), block.signature, forger.walletPublicKey);
    bool blockHasTransactions = blockchain->blockHasTransactions(block);

    if (lastBlockHashValid && forgerValid && transactionValid && signatureValid && blockHasTransactions) {
        blockchain->addBlock(block);
        transactionPool.removeFromPool(block.transactions);
        
        /*! 
        \todo write the new block to our mongodb instance
        */

        if (broadcast) {
            Message message("BLOCK", block.toJson());
            std::string msgJson = message.toJson();
            p2p->broadcast(msgJson.c_str());
        }
    } else {
        std::cerr << "Block verification failed" << std::endl;
        std::cerr << "lastBlockHashValid: " << std::boolalpha << lastBlockHashValid << std::endl;
        std::cerr << "forgerValid: " << std::boolalpha << forgerValid << std::endl;
        std::cerr << "transactionValid: " << std::boolalpha << transactionValid << std::endl;
        std::cerr << "signatureValid: " << std::boolalpha << signatureValid << std::endl;
        std::cerr << "blockHasTransactions: " << std::boolalpha << blockHasTransactions << std::endl;
    }
}

/*!
Request complete or partial blockchain from the master node. Uses the local block count to determine
how many blocks are needed from the master server.
*/
void Node::requestChain() {
    // std::cout << "inside requestChain()" << std::endl;
    std::string requestingNode { p2p->sc.ip + ":" + std::to_string(p2p->sc.port) + ":" + std::to_string(blockchain->blocks.size()) };
    Message message("BLOCKCHAINREQUEST", requestingNode);
    std::string msgJson = message.toJson();

    // int outgoingSocket = p2putils::setOutgoingNodeConnection(utils::MASTER_NODE_IP, utils::MASTER_NODE_PORT);
    // if (outgoingSocket == -1) {
    //     return;
    // }
    p2p->broadcast(msgJson.c_str());
}

/*!
Only send the blocks which are missing from the requesting node.
*/
void Node::handleBlockchainRequest(std::string requestingNode) {
    // std::lock_guard<std::mutex> guard(blockchain->blockchainMutex);
    /* 
    Only get the blocks I need. [X]
    */
   // std::cout << "Inside handleBlockchainRequest()" << std::endl;
   if (blockchain->blocks.size() == 1) {
       return;
   }

    std::vector<std::string> receivingNode = utils::split(requestingNode, ":");
    // int blockNumber = atoi(receivingNode.at(2).c_str());
    // std::cout << "requesting from block: " << blockNumber << std::endl;
    
    // vector<Block> subvector = {blockchain->blocks.begin(), blockchain->blocks.end()};
    // std::cout << "Sending: " << blockchain->toJsonString(subvector) << std::endl;

    Message message("BLOCKCHAIN", blockchain->toJsonString());
    std::string msgJson = message.toJson();
    
    int num = atoi(receivingNode.at(1).c_str());
    int outgoingSocket = p2putils::setOutgoingNodeConnection(receivingNode.at(0), num);
    if (outgoingSocket == -1) {
        return;
    }
    p2p->send_node_message(outgoingSocket, msgJson.c_str());
}

/*!
This is where we read and rebuild the blockchain or partial blockchain after requesting blocks
from the master server. How many blocks we ad is based on how many we requested.
*/
void Node::handleBlockchain(std::string blockchainString) {
    // std::cout << "inside handleBlockchain(): " << blockchainString << std::endl;

    // std::lock_guard<std::mutex> guard(blockchain->blockchainMutex);
    if (blockchainString.empty())
        return;

    // std::cout << "blockchainString: " << blockchainString << std::endl;

    nlohmann::json j;
    try {
        j = nlohmann::json::parse(blockchainString);
    } catch(exception &e) {
        std::cout << "handleBlockchain() " << blockchainString << std::endl;
        std::cerr << e.what() << std::endl;
    }
    int localBlockCount = blockchain->blocks[blockchain->blocks.size()-1].blockCount;
    int receivedBlockCount = j["blocks"].size();

    std::cout << "localBlockCount: " << localBlockCount << std::endl;
    std::cout << "receivedBlockCount: " << receivedBlockCount << std::endl;

    if (receivedBlockCount > localBlockCount) {
        std::cout << "replacing chain" << std::endl;
        
        int blockNumber {0};
        std::vector<Block> localBlockchainCopy = blockchain->blocks;
        for (auto& element : j["blocks"]) {
            // std::cout << "\n\nelement: " << element << "\n" << std::endl;
            blockNumber++;
            Block b;
            b.blockCount = element["blockCount"];
            b.forgerAddress = element["forgerAddress"];
            b.hash = element["hash"];
            b._id = element["_id"];
            b.lastHash = element["lastHash"];
            b.signature = element["signature"];
            b.timestamp = element["timestamp"];
            std::vector<Transaction> transactions;
            for (auto& tx : element["transactions"]) {
                Transaction t;
                t.id = tx["id"];
                t.amount = tx["amount"];
                t.receiverAddress = tx["receiverAddress"];
                t.senderAddress = tx["senderAddress"];
                t.signature = tx["signature"];
                t.timestamp = tx["timestamp"];
                t.type = tx["type"];
                transactions.push_back(t);
                accountModel->updateBalance(t.senderAddress, -t.amount);
                accountModel->updateBalance(t.receiverAddress, t.amount);
            }
            b.transactions = transactions;
            localBlockchainCopy.push_back(b);
            /*! 
            \todo write the new block to our mongodb instance
            */
        }
        blockchain->blocks = localBlockchainCopy;
    }
}

/*!
This is where new blocks are initiated for this forger.
*/
void Node::forge() {
    std::string forger = blockchain->nextForger();
    if (forger == nodeWallet->walletPublicKey) {
        std::cout << "i am the next forger" << std::endl;
        try {

            std::vector<Transaction> rewardedTransactions = blockchain->calculateForgerReward(transactionPool.transactions);
            Block block = blockchain->createBlock(rewardedTransactions, nodeWallet->address); 
            transactionPool.removeFromPool(block.transactions);

            /*! 
            \todo write the new block to our mongodb instance
            */

            Message message("BLOCK", block.toJson());
            std::string msgJson = message.toJson();
            p2p->broadcast(msgJson.c_str());
        } catch (std::exception &e) {std::cerr << "exception: " << e.what() << std::endl; }
    } else {
        // std::cout << "i am not the next forger" << std::endl;
        std::string address = utils::generateAddress(forger);
        accountModel->addAccount(address, forger);
    }
}