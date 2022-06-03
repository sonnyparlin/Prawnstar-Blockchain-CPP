#include "SocketCommunication.hpp"
#include "Message.hpp"
#include "Block.hpp"
#include "Transaction.hpp"
#include <nlohmann/json.hpp>
#include <typeinfo>
#include <algorithm>
#include <fstream>

/*!
 *
 * @param node
 *
 * Constructor for SocketCommunication class
 */
SocketCommunication::SocketCommunication(Node *node) {
    this->node = node;
}

SocketCommunication::~SocketCommunication()=default;


/*!
 *
 * @param argc
 * @param argv
 *
 * Calls processArgs and then starts peer to peer operations.
 */
void SocketCommunication::startSocketCommunication(int argc, char *argv[]) {
    
    if (utils::processArgs(argc, argv) != 0)
        exit(1);

    startP2POperations(argc, argv);
}

/*!
 *
 * @param argc
 * @param argv
 *
 * This is where everything starts up in terms of a single node. We start with
 * our p2p server, our peer discovery status thread as well as our actual peer discovery
 * thread. We also check to see if this is a master node or not, if not, it will grab
 * the blocks it's missing (if any) from the blockchain on the master node.
 */
void SocketCommunication::startP2POperations( int argc, char **argv ) {
    sc.ip = argv[1];
    sc.port = std::stoi(argv[2]);

    std::thread serverThread (&SocketCommunication::startP2PServer, this, argc, argv);
    serverThread.detach();

    std::thread statusThread (&SocketCommunication::peerDiscoveryStatus, this);
    statusThread.detach();

    std::thread discoveryThread (&SocketCommunication::peerDiscovery, this);
    discoveryThread.detach();

    std::thread forgerThread (&SocketCommunication::blockForger, this);
    forgerThread.detach();
}

/*!
 *
 * @param argc
 * @param argv
 *
 * Starts the peer to peer server.
 */
[[noreturn]] void SocketCommunication::startP2PServer ( int argc, char **argv )
{
    P2P::Server server = P2P::startOnPort(argv[2]);
    std::cout << "Server created with id: " << server.id << std::endl;
    int i=0;

    for (;;) {
        int outgoingSocket;
        int incomingSocket;

        if ((argc == 5 && i !=0) || argc == 4) {
            incomingSocket = Accept(server);
        }

        // Master node handshake
        if (i == 0 && argc == 5) {
            outgoingSocket = P2P::setOutgoingNodeConnection(utils::get_master_node_ip(),
                                                            utils::get_master_node_port());
            std::thread peerThread (&SocketCommunication::handshake, this, outgoingSocket);
            peerThread.join();
        }

        // Server operations
        if ((argc == 5 && i > 0) || argc == 4) {
            std::thread peerThread (&SocketCommunication::receive_node_message, this, incomingSocket);
            peerThread.join();
        }
        i++;
    }
}

/*!
 * blockforger polls the transaction pool once every 300 seconds and if there
 * are transactions in the pool, it calls forge() so they are dealt with.
 */
[[noreturn]] void SocketCommunication::blockForger() {
    for(;;) {
        std::cout << "Forger Polling... " << std::endl;
        sleep(30); // 300 seconds between blocks
        if (!node->blockchain->transactionPool.transactions.empty()) {
            std::cout << node->blockchain->transactionPool.transactions.size() << " transactions" << std::endl;
            std::cout << "Forging new block..." << std::endl;
            node->forge();
        }
    }
}

/*!
 * PeerDiscoveryStatus is used to display connected nodes.
 */
[[noreturn]] void SocketCommunication::peerDiscoveryStatus() {
    for(;;) {
        std::cout << "Current connections: " << std::endl;
        if (!peers.empty()) {
            //std::cout << "There are " << peers.size() << " peers connected" << std::endl;
            for( auto const& peer : peers ) {
                std::cout << peer << std::endl;
            }
        } else
            std::cout << "No peers connected" << std::endl;

        sleep(10);
    }
}

/*!
 * The actual peer discovery system. It creates a handshake message and broadcasts it.
 */
[[noreturn]] void SocketCommunication::peerDiscovery() {
    for(;;) {
        std::string message = handshakeMessage();
        broadcastPeerDiscovery(message.c_str());
        sleep(10);
    }
}

/*!
 *
 * @param message
 *
 * Peer dscovery thread for discovering other nodes on the network.
 */
void SocketCommunication::broadcastPeerDiscovery(const char *message) {
    nlohmann::json j;
    try {j = nlohmann::json::parse(message + MESSAGELENGTH);
    } catch (std::exception &e) { std::cerr << e.what() << std::endl; }
    if (j["Message"]["Peers"] == nullptr) return;

    std::vector<std::string> dest {};
    dest.assign(std::begin(j["Message"]["Peers"]), std::end(j["Message"]["Peers"]));

    std::vector<std::string> peersPeerList {dest};
    std::for_each(peersPeerList.begin(), peersPeerList.end(), [this](auto &ipPortStr){
        std::vector<std::string> ipPortStrV = utils::split(ipPortStr, ":");
        auto num = (int)stol(ipPortStrV.at(1));
        int outgoingSocket = P2P::setOutgoingNodeConnection(ipPortStrV.at(0), num);
        if (outgoingSocket == -1) {

            // Socket connection to peer failed, remove peer from peers list.
            std::string elementToRemove = ipPortStrV.at(0) + ":" + std::to_string(num);
            std::string masterNode = utils::get_master_node_ip();
            std::string masterNode2 = ":" + std::to_string(utils::get_master_node_port());
            masterNode += masterNode2;

            if (elementToRemove != masterNode) {
                peers.erase(std::remove(peers.begin(), peers.end(), elementToRemove), peers.end());
                inactivePeers.push_back(elementToRemove);
            }
            return;
        }

        SocketCommunication::handshake(outgoingSocket);
        P2P::Close(outgoingSocket);
    });
}

/*!
 *
 * @param message
 *
 * Broadcast a message to all known peers
 */
void SocketCommunication::broadcast(const char *message) const {
    if (peers.empty()) {return;}
    auto fullJsonString = nlohmann::json::parse(message + MESSAGELENGTH);

    std::vector<std::string> tempPeers = peers;
    std::for_each(tempPeers.begin(), tempPeers.end(), [&message](auto &ipPortStr){

        std::vector<std::string> ipPortStrV = utils::split(ipPortStr, ":");
        auto num = (int)stol(ipPortStrV.at(1));
        int outgoingSocket = P2P::setOutgoingNodeConnection(ipPortStrV.at(0), num);
        if (outgoingSocket == -1)
            return;
        send_node_message(outgoingSocket, message);
        P2P::Close(outgoingSocket);
    });
}

/*!
 *
 * @param sock
 * @param message
 *
 * Used for sending a specific node (connected via our socket) a message.
 */
void SocketCommunication::send_node_message(int sock, const char *message) {
    auto result = send(sock, message, static_cast<int>(strlen(message)), 0);
    if (result < 0) {
        P2P::handleError("send() error ");
        return;
    }
}

/*!
 *
 * @param sock
 *
 * Recieves a message sent from the node connected to our socket. We make two calls to recv(),
 * the first call gets the message length, the second call gets the message.
 */
void SocketCommunication::receive_node_message(int sock) {
    const char* buffer = P2P::Recv(sock);
    handleNodeMessage(buffer);
    P2P::Close(sock);
    delete[] buffer;
}

/*!
 *
 * @param sock
 *
 * Send the actual handshake to the node comnected to our socket.
 */
void SocketCommunication::handshake(int sock) const {
    std::string message = handshakeMessage();
    send_node_message(sock, message.c_str());
}

/*!
 *
 * @return std::string
 *
 * Create the handshakeMessage for peer discovery.
 */
std::string SocketCommunication::handshakeMessage() const {
    nlohmann::json j;
    std::string objectAsString;
    std::string messageType = "DISCOVERY";
    Message handshakeMsg(sc, messageType, peers);
    std::string jsonMessage = handshakeMsg.toJson();
    return jsonMessage;
}

/*!
 *
 * @param message
 *
 * Handle the incoming peer discovery message.
 */
void SocketCommunication::peerDiscoveryHandleMessage(nlohmann::json &j) {
    SocketConnector peersSenderConnector(
        j["Message"]["SocketConnector"]["ip"],
        int(j["Message"]["SocketConnector"]["port"])
    );

    bool newPeerFlag = true;
    if (!peers.empty()) {
        std::for_each(peers.begin(), peers.end(),
                                    [&peersSenderConnector, &newPeerFlag](auto const &alreadyConnectedPeer) {
            std::string newPeer = peersSenderConnector.ip + ":" + std::to_string(peersSenderConnector.port);
            if (newPeer == alreadyConnectedPeer)
                newPeerFlag = false;
        });
    }

    if (newPeerFlag) {
        std::string newPeerToAdd;
        std::cout << "Adding peer via peer discovery: " << peersSenderConnector.ip << ":"
                  << peersSenderConnector.port << std::endl;
        newPeerToAdd = peersSenderConnector.ip + ":" + std::to_string(peersSenderConnector.port);
        peers.push_back(newPeerToAdd);
        std::cout << "Requesting blockchain from network" << std::endl;
        node->requestChain();
    }

    if (j["Message"]["Peers"] == nullptr)
        return;

    std::vector<std::string> dest;
    dest.assign(std::begin(j["Message"]["Peers"]), std::end(j["Message"]["Peers"]));
    std::vector<std::string> peersPeerList {dest};
    std::for_each(peersPeerList.begin(), peersPeerList.end(), [this](auto &peersPeer) {
        bool peerKnown = false;
        std::for_each (peers.begin(), peers.end(), [&peersPeer, &peerKnown](auto const &p){
            if (peersPeer == p)
                peerKnown = true;
        });

        std::string thisNode = sc.ip + ":" + std::to_string(sc.port);
        if (!peerKnown && peersPeer != thisNode) {
            std::vector<std::string> tcpPair = utils::split(peersPeer, ":");
            auto num = (int)stol(tcpPair.at(1));
            std::string peerToCheck = tcpPair.at(0) + ":" + std::to_string(num);

            // make sure new peer isn't an inactive peer before adding
            if (std::find(inactivePeers.begin(),
                          inactivePeers.end(), peerToCheck) == inactivePeers.end()) {
                std::cout << "Adding peer via peer discovery: "
                          << tcpPair.at(0)
                          << ":"
                          << std::to_string(num)
                          << std::endl;
                std::string newPeerToAdd = tcpPair.at(0) + ":" + std::to_string(num);
                peers.push_back(newPeerToAdd);
                inactivePeers.clear();
            }
        }
    });
}

void SocketCommunication::transactionHandler(const std::string& transactionString) {
    nlohmann::json jx;
    try { jx = nlohmann::json::parse(transactionString);
    } catch(std::exception &e) { std::cout << e.what() << std::endl; }

    Transaction itx;
    itx.id = jx["id"];
    itx.amount = jx["amount"];
    itx.senderAddress = jx["senderAddress"];
    itx.senderPublicKey = jx["senderPublicKey"];
    itx.receiverAddress = jx["receiverAddress"];
    itx.signature = jx["signature"];
    itx.timestamp = jx["timestamp"];
    itx.type = jx["type"];

    node->handleTransaction(itx, false);
}

void SocketCommunication::blockHandler(const std::string& data) {
    Block block;
    nlohmann::json jx;
    try { jx = nlohmann::json::parse(data);
    } catch(std::exception &e) { std::cerr << e.what() << std::endl; }

    std::for_each(jx["transactions"].begin(), jx["transactions"].end(), [this, &block](const auto& itx){
        Transaction tr;
        tr.id = itx["id"];
        tr.amount = itx["amount"];
        tr.senderAddress = itx["senderAddress"];
        Wallet senderWallet(tr.senderAddress.c_str(), this->node);
        tr.senderPublicKey = senderWallet.walletPublicKey;
        tr.receiverAddress = itx["receiverAddress"];
        tr.signature = itx["signature"];
        tr.timestamp = itx["timestamp"];
        tr.type = itx["type"];
        block.transactions.push_back(tr);
    });
    block.lastHash = jx["lastHash"];
    block.hash = jx["hash"];
    block.forgerAddress = jx["forgerAddress"];
    block.timestamp = jx["timestamp"];
    block.blockCount = jx["blockCount"];
    block.signature = jx["signature"];

    node->handleBlock(block, false);
}

void SocketCommunication::handleNodeMessage(const char* buffer) {
    nlohmann::json j;
    try { j = nlohmann::json::parse(buffer);
    } catch(std::exception &e) { std::cerr << e.what() << std::endl; }
    std::string messageType = j["Message"]["Type"];

    if (messageType == "DISCOVERY")
        peerDiscoveryHandleMessage(j);
    else if (messageType == "TRANSACTION")
        transactionHandler(j["Message"]["data"]);
    else if (messageType == "BLOCK")
        blockHandler(j["Message"]["data"]);
    else if (messageType == "BLOCKCHAINREQUEST")
        node->handleBlockchainRequest(j["Message"]["data"]);
    else if (messageType == "BLOCKCHAIN")
        node->handleBlockchain(j["Message"]["data"]);
}