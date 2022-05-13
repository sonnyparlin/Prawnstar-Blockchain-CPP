#include "Node.hpp"
#include "NodeApi.hpp"
#include <crow.h>
#include <thread>
#include <ctime>

Node* Node::node=nullptr;

Node::Node(char **argv) {
    p2p = new SocketCommunication(this);
    accountModel = new AccountModel();
    blockchain = new Blockchain(this);

    int port = utils::getPort(argv[2]);
    if (port == 10001)
        nodeWallet = new Wallet(this, "genesisNode.pem");
    else if (port == 10002)
        nodeWallet = new Wallet(this, "node2Wallet.pem");
    else
        nodeWallet = new Wallet(this);
    
    node2Wallet = new Wallet(this, "node2Wallet.pem");
    exchangeWallet = new Wallet(this, "exchange.pem");
    aliceWallet = new Wallet(this, "alice.pem");
    bobWallet = new Wallet(this, "bob.pem");
    proofOfStake = new ProofOfStake(this);
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

/*!
 *
 * @param argv
 * @return Node
 *
 * Our main singleton Node for the application.
 */
Node *Node::createNode(char **argv) {
    if (node == nullptr) {
        node = new Node(argv);
    }

    return node;
}

/*!
 *
 * @param argc
 * @param argv
 *
 * Start peer to peer server and api server
 */
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
 *
 * @return std::string
 *
 * Return Node's peer to peer server id
 */
std::string Node::getNodeID() const {
    return p2p->id;
}

/*!
 *
 * @param filename
 * @param lineCount
 * @return std::string
 *
 * Get the last 100 lines of log file, downloaded function.
 */
std::string getLastLines( std::string const& filename, int lineCount )
{
    size_t const granularity = 100 * lineCount;
    std::ifstream source( filename.c_str(), std::ios_base::binary );
    source.seekg( 0, std::ios_base::end );
    size_t size = static_cast<size_t>( source.tellg() );
    std::vector<char> buffer;
    int newlineCount = 0;
    while ( source 
            && buffer.size() != size
            && newlineCount < lineCount ) {
        buffer.resize( std::min( buffer.size() + granularity, size ) );
        source.seekg( -static_cast<std::streamoff>( buffer.size() ),
                      std::ios_base::end );
        source.read( buffer.data(), (long)buffer.size() );
        newlineCount = (int)std::count( buffer.begin(), buffer.end(), '\n');
    }
    auto start = buffer.begin();
    while ( newlineCount > lineCount ) {
        start = std::find( start, buffer.end(), '\n' ) + 1;
        -- newlineCount;
    }
    auto end = remove( start, buffer.end(), '\r' );
    return {start, end };
}

/*!
 *
 * @param msg
 *
 * Write to log file.
 */
void Node::log(std::string const& msg)
{
    std::lock_guard<std::mutex> guard(logMutex);
    ofstream myfile;
    myfile.open ("console.log", std::ios_base::app);
    myfile << msg << std::endl;
    myfile.close();
}

/*!
 *
 * @return std::string
 *
 * Return last 10 lines of console.log
 */
std::string Node::getConsoleLog() {
    return getLastLines("console.log", 100);
}

/*!
 *
 * @param transaction
 * @param broadcast
 * @return
 *
 * This is where we handle transactions. Called from NodeAPI and SocketCommunication.
 */
bool Node::handleTransaction (Transaction &transaction, bool broadcast ) {
    bool transactionCovered = false;
    bool signatureValid = utils::verifySignature(transaction.payload(),
                                                 transaction.signature,
                                                 transaction.signatureLength,
                                                 transaction.senderPublicKey);
    if (!signatureValid)
        return false;

    bool transactionExists = transactionPool.transactionExists(transaction);
    if (transactionExists)
        return false;

    bool transactionInBlockChain = blockchain->transactionExists(transaction);
    if (transactionInBlockChain)
        return false;
    
    if (transaction.type != "EXCHANGE") {
        if (accountModel->getBalance(transaction.senderAddress) >= transaction.amount)
            transactionCovered = true;
    } else
        transactionCovered = true;

    if (!transactionCovered)
        return false;

    transactionPool.addTransaction(transaction);

    if (broadcast) {
        // std::cout << "broadcasting tx: " << transaction.toJson() << std::endl;
        std::string msgType = "TRANSACTION";
        std::string msgBody = transaction.toJson();
        Message message(msgType, msgBody);
        std::string msgJson = message.toJson();
        p2p->broadcast(msgJson.c_str());
    }

    bool forgingRequired = transactionPool.forgerRequired();
    if (forgingRequired) forge();

    return true;
}

/*!
 *
 * @param block
 * @param broadcast
 * Do check to make sure the block is valid before adding it to the chain.
 * Broadcast if necessary.
 */
void Node::handleBlock (Block &block, bool broadcast) {
    Wallet forger(block.forgerAddress.c_str(), this);
    std::string blockHash = block.hash;
    bool blockCountValid = blockchain->blockCountValid(block);

    // Requesting node is not up-to-date, so let's give them the blocks they're missing.
    if (!blockCountValid) {
        requestChain();
        return;     
    }

    bool lastBlockHashValid = blockchain->lastBlockHashValid(block);
    bool forgerValid = blockchain->forgerValid(block);
    bool transactionValid = blockchain->transactionValid(block.transactions);
    bool signatureValid = utils::verifySignature(block.payload(),
                                                 block.signature,
                                                 block.signatureLength,
                                                 forger.walletPublicKey);
    bool blockHasTransactions = blockchain->blockHasTransactions(block);

    if (lastBlockHashValid && forgerValid && transactionValid && signatureValid &&
                                                                blockHasTransactions) {
        blockchain->addBlock(block);
        transactionPool.removeFromPool(block.transactions);
        
        /*! 
        \todo write the new block to our mongodb instance
        */

        if (broadcast) {
            std::string msgType = "block";
            std::string msgBody = block.toJson();
            Message message(msgType, msgBody);
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
 * Request complete or partial blockchain from the master node. Uses the local
 * block count to determine how many blocks are needed from the master server.
 */
void Node::requestChain() const {
    std::string requestingNode { p2p->sc.ip + ":" +
                                 std::to_string(p2p->sc.port) + ":" +
                                 std::to_string(blockchain->blocks.size()) };
    std::string msgType = "BLOCKCHAINREQUEST";
    Message message(msgType, requestingNode);
    std::string msgJson = message.toJson();
    p2p->broadcast(msgJson.c_str());
}

/*!
 *
 * @param requestingNode
 *
 * Only send the blocks which are missing from the requesting node.
 */
void Node::handleBlockchainRequest(std::string requestingNode) const {
    std::lock_guard<std::mutex> guard(blockchain->blockchainMutex);
    /* 
    Only get the blocks I need. [X]
    */
   // std::cout << "Inside handleBlockchainRequest()" << requestingNode << std::endl;
   if (blockchain->blocks.size() == 1) {
       return;
   }

    std::vector<std::string> receivingNode = utils::split(requestingNode, ":");
    auto blockNumber = stol(receivingNode.at(2));

    if (blockNumber > static_cast<int>(blockchain->blocks.size()))
        return;

    std::cout << "\n\n\tReceived blockchain download request from "
              << receivingNode.at(0) + " for "
              << (blockchain->blocks.size() - blockNumber)
              << " blocks.\n"
              << std::endl;

    vector<Block> subvector = {blockchain->blocks.begin() + blockNumber, blockchain->blocks.end()};
    std::string msgType = "BLOCKCHAIN";
    std::string msgBody = Blockchain::toJsonString(subvector);
    Message message(msgType, msgBody);
    std::string msgJson = message.toJson();

    auto num = static_cast<int>(stol(receivingNode.at(1)));
    int outgoingSocket = p2putils::setOutgoingNodeConnection(receivingNode.at(0), num);
    if (outgoingSocket == -1) {
        return;
    }
    SocketCommunication::send_node_message(outgoingSocket, msgJson.c_str());
}

/*!
 *
 * @param blockchainString
 *
 * This is where we read and rebuild the blockchain or partial blockchain after requesting blocks
 * from the master server. How many blocks we ad is based on how many we requested.
 */
void Node::handleBlockchain(const std::string &blockchainString) const {
    std::lock_guard<std::mutex> guard(blockchain->blockchainMutex);
    if (blockchainString.empty())
        return;

    nlohmann::json j;
    try {
        j = nlohmann::json::parse(blockchainString);
    } catch(exception &e) {
        std::cout << "handleBlockchain() " << blockchainString << std::endl;
        std::cerr << e.what() << std::endl;
    }
    auto localBlockCount = (int)blockchain->blocks[blockchain->blocks.size()-1].blockCount;
    auto receivedBlockCount = (int)j["blocks"].size();

    std::cout << "localBlockCount: " << localBlockCount << std::endl;
    std::cout << "receivedBlockCount: " << receivedBlockCount << std::endl;

    if (receivedBlockCount != localBlockCount) {
        std::cout << "importing blocks" << std::endl;

        int blockNumber {0};
        std::vector<Block> localBlockchainCopy = blockchain->blocks;
        for (auto& element : j["blocks"]) {
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
            for (auto& itx : element["transactions"]) {
                Transaction t;
                t.id = itx["id"];
                t.amount = itx["amount"];
                t.receiverAddress = itx["receiverAddress"];
                t.senderAddress = itx["senderAddress"];
                t.senderPublicKey = accountModel->addressToPublicKey[t.senderAddress];
                t.signature = itx["signature"];
                t.timestamp = itx["timestamp"];
                t.type = itx["type"];
                transactions.push_back(t);

                if (t.type == "STAKE") {
                    accountModel->updateBalance(t.senderAddress, -t.amount);
                    proofOfStake->update(t.senderPublicKey, t.amount);
                } else {
                    accountModel->updateBalance(t.senderAddress, -t.amount);
                    accountModel->updateBalance(t.receiverAddress, t.amount);
                }
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
 *
 * @return std::string
 *
 * Return the current time as a string.
 */
std::string Node::getTimeStr()
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
    auto str = oss.str();
    return str;
}

/*!
 * This is where new blocks are initiated for this forger.
 */
void Node::forge() {
    /*!
     * Return the new block forger based on the proof of work algorithm.
     */
    std::string forger = blockchain->nextForger();

    /*! if there are more than 100 transactions in the transaction pool
     * assume we are having connection problems with the current staker's node
     * and remove that staker from the stakers vector.
     */
    if (transactionPool.transactions.size() > 100 && forger != proofOfStake->genesisNodeStake) {

        /*!
         * 100 transactions in the transaction pool means thee is a clog in the system,
         * such a clog will be in the form of a node that has staked prawn but the node is unreachable
         * and so transactions are piling up in the transaction pool. So let's get the address of this
         * staker, return the stake back to the staker's wallet and then remove the staker from the
         * list of valid stakers. Then re-run the proof of stake lottery to find a new staker and proceed
         * with forging the new block.
         */
        std::string addr = "pv1" + utils::generateAddress(forger);
        accountModel->updateBalance(addr, proofOfStake->stakers[forger]);
        proofOfStake->stakers.erase(forger);

        /*!
         * At 500 transactions in the pool, we know it's time to clean up.
         * There should never be this many transactions in the transaction pool.
         * This can only happen if the node was identified as a staker but wasn't
         * able to forge() new blocks, perhaps due to network connectivity issues
         * or some other reason. Either way, we can safely assume these are dead
         * transactions or they've been processed already.
         */
        if (transactionPool.transactions.size() > 500) {
            if (proofOfStake->stakers.count(forger) == 0)
                transactionPool.transactions.clear();
        }

        /*!
         * Okay now that we've removed the broken staker, let's try
         * to pick a new staker.
         */
        forger = blockchain->nextForger();
    }

    auto timeStr = getTimeStr();
    if (forger == nodeWallet->walletPublicKey) {
        log(timeStr);
        log("i am the next forger");
        Block block;
        std::vector<Transaction> rewardedTransactions;
        
        try {
            rewardedTransactions = blockchain->calculateForgerReward(transactionPool.transactions);
        } catch (std::exception &e) {
            std::cerr << "reward transaction exception: " << e.what() << std::endl;
        }
        
        try {
            block = blockchain->createBlock(rewardedTransactions, nodeWallet->address);
        } catch (std::exception &e) {
            std::cerr << "creaate block exception: " << e.what() << std::endl;
        }
        transactionPool.removeFromPool(block.transactions);
        
        /*! 
        \todo write the new block to our mongodb instance
        */

        std::string msgType = "BLOCK";
        std::string msgBody = block.toJson();
        Message message(msgType, msgBody);
        std::string msgJson = message.toJson();
        p2p->broadcast(msgJson.c_str());
    } else {
        log(timeStr);
        log("i am not the next forger");
        std::string address = utils::generateAddress(forger);
        accountModel->addAccount(address, forger);
    }
}