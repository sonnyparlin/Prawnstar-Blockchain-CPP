#include "SocketCommunication.hpp"
#include "Message.hpp"
#include "Block.hpp"
#include "Transaction.hpp"
#include <nlohmann/json.hpp>
#include <typeinfo>
#include <algorithm>
#include <fstream>

SocketCommunication::SocketCommunication(Node *node) {
    this->node = node;
}

SocketCommunication::~SocketCommunication()=default;

void SocketCommunication::startSocketCommunication(int argc, char *argv[]) {
    
    if (processArgs(argc, argv) != 0)
        exit (1);

    startP2POperations(argc, argv);
}

void SocketCommunication::inbound_node_connected(int sock) {
    receive_node_message(sock);
}

void SocketCommunication::outbound_node_connected(int sock) {
    handshake(sock);
}

void SocketCommunication::send_node_message(int sock, const char *message) {
    std::string substr = message;
    // std::cout << "Message length: " << substr.substr(0,20)  << std::endl;
    // std::cout << message << std::endl;
    auto result = send(sock, message, strlen(message), 0);
    if (result < 0) {
        #ifndef _WIN32
        std::cout << "send() error " << errno << std::endl;
        #else
        std::cout << "\nsend() error: " << WSAGetLastError() << std::endl;
        #endif
        return;
    }
}

void SocketCommunication::receive_node_message(int sock) {
    // read message body length
    unsigned long long msgLength;
    char msgLengthBuffer[MESSAGELENGTH];
    
    auto reader = recv (sock, msgLengthBuffer, MESSAGELENGTH, MSG_WAITALL);
    if (reader < 0) {
        #ifndef _WIN32
        std::cout << "recv() error " << errno << std::endl;
        #else
        std::cout << "\nrecv() error: " << WSAGetLastError() << std::endl;
        #endif
        return;
    }

    msgLength = stoi (msgLengthBuffer);
    // std::cout << "Message Length => " << msgLength << std::endl;
    if (msgLength <= 0) {
        std::cout << "Message length is not valid, errno: " << errno << std::endl;
        return;
    }

    // allocate memory with message's length
    char* buffer = new char[msgLength + 1]();

    // read socket data to the allocated buffer
    reader = recv (sock, buffer, msgLength, MSG_WAITALL);
    // std::cout << "reader => " << reader << std::endl;
    if (reader <= 0) {
        if (close(sock) == -1) {
            std::cout << "Close problems, errno: " << errno << std::endl;
        }
    }
    
    // std::cout << "buffer: " << buffer << std::endl;
    nlohmann::json j;
    
    try {
        j = nlohmann::json::parse(buffer);
    } catch(exception &e) {
        std::cerr << e.what() << std::endl;
    }
    std::string messageType = j["Message"]["Type"];

    // std::cout << "Message type: " << messageType << std::endl;

    if (messageType == "DISCOVERY") {
        peerDiscoveryHandleMessage(buffer);
    } else if (messageType == "TRANSACTION") {
        std::string transactionString = j["Message"]["data"];
        auto jx = nlohmann::json::parse(transactionString);
        
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
    } else if (messageType == "BLOCK") {
        Block block;
        std::string jsonBlock = j["Message"]["data"];
        nlohmann::json jx;

        try {
            jx = nlohmann::json::parse(jsonBlock);
        } catch(exception &e) {
            std::cerr << e.what() << std::endl;
        }
        
        for (auto itx : jx["transactions"]) {
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
        }
        block.lastHash = jx["lastHash"];
        block.hash = jx["hash"];
        block._id = block.hash;
        block.forgerAddress = jx["forgerAddress"];
        block.timestamp = jx["timestamp"];
        block.blockCount = jx["blockCount"];
        block.signature = jx["signature"];
        // std::cout << "block signature: " << block.signature << std::endl;

        node->handleBlock(block, false);
    } else if (messageType == "BLOCKCHAINREQUEST") {
        std::string requestingNode = j["Message"]["data"];
        node->handleBlockchainRequest(requestingNode);
    } else if (messageType == "BLOCKCHAIN") {
        std::string messageData = j["Message"]["data"];
        node->handleBlockchain(messageData);
    }

    // delete allocated memory
    #ifndef _WIN32
    int returnVal = close(sock);
    #else
    int returnVal = closesocket(sock);
    #endif
    if (returnVal != 0) {
        #ifndef _WIN32
        std::cout << "close() error " << errno << std::endl;
        #else
        std::cout << "\nclose() error: " << WSAGetLastError() << std::endl;
        #endif
    }
    delete[] buffer;
}

int SocketCommunication::processArgs(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "Usage for the first node: ./server <ip> <port>" << std::endl;
        return 1;
    }

    if (argc < 4) {
        std::cout << "Node usage for peers: ./server <ip> <port> <masternode> <api_port>" << std::endl;
        std::cout << "If this is not the master node, restart with 3rd parameter" << std::endl;
    }
    
    auto arg = stoi(argv[2]);

    if (arg < INT_MIN || arg > INT_MAX) {
        std::cout << "Invalid port" << std::endl;
        return 1;
    }

    if (!p2putils::isValidIpAddress(argv[1])) {
        std::cout << "Invalid ip address in process args" << std::endl;
        return 1;
    }
    return 0;
}

/*!
This is where everything starts up in terms of a single node. We start with
our p2p server, our peer discovery status thread as well as our actual peer discovery
thread. We also check to see if this is a master node or not, if not, it will grab 
the blocks it's missing (if any) from the blockchain on the master node.
*/
void SocketCommunication::startP2POperations( int argc, char **argv ) {
    sc.ip = argv[1];
    sc.port = stoi(argv[2]);

    std::thread serverThread (&SocketCommunication::startP2PServer, this, argc, argv);
    serverThread.detach();

    std::thread statusThread (&SocketCommunication::peerDiscoveryStatus, this);
    statusThread.detach();

    std::thread discoveryThread (&SocketCommunication::peerDiscovery, this);
    discoveryThread.detach();

    std::thread forgerThread (&SocketCommunication::blockForger, this);
    forgerThread.detach();
}

void SocketCommunication::startP2PServer ( int argc, char **argv )
{
    auto PORT = stoi(argv[2]);
    id = utils::get_uuid();
    struct sockaddr_in address{};
    int address_length = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 

    /*! 
    \todo open db and if it has no data, write the genesis block to our mongodb instance
    */

    // Server initialization
    int serverSocket = p2putils::createSocket();    
    if (!p2putils::Bind(serverSocket, address, PORT))
        exit (1);

    int i=0;
    std::cout << "Server created with id: " << id << std::endl;

    for (;;) {
        int outgoingSocket;
        int incomingSocket;

        if ((argc == 5 && i !=0) || argc == 4) {
            if ((incomingSocket = accept(serverSocket,
                (struct sockaddr *)&address,
                (socklen_t*)&address_length)) < 0) {
                    #ifndef _WIN32
                    std::cout << "accept() error " << errno << std::endl;
                    #else
                    std::cout << "\naccept() error: " << WSAGetLastError() << std::endl;
                    #endif
                    exit(0);
            }
        }

        //=========================================================
        // Master node handshake
        //=========================================================   
        if (i == 0 && argc == 5) {
            outgoingSocket = p2putils::setOutgoingNodeConnection(argv[3], utils::get_master_node_port());
            std::thread peerThread (&SocketCommunication::outbound_node_connected, this, outgoingSocket);
            peerThread.join();
        }

        //================================================================
        // Server operations
        //================================================================
        if (argc == 5 && i > 0) {
            std::thread peerThread (&SocketCommunication::inbound_node_connected, this, incomingSocket);
            peerThread.join();
        } else if (argc == 4) {
            std::thread peerThread (&SocketCommunication::inbound_node_connected,this, incomingSocket);
            peerThread.join();
        }
        i++;
    }
}


//==============================
// Peer Discovery Methods
//==============================

void SocketCommunication::peerDiscoveryStatus() {
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

void SocketCommunication::blockForger() {
    for(;;) {
        std::cout << "Forger Polling... " << std::endl;

        sleep(300); // 300 seconds between blocks
        if (!node->transactionPool.transactions.empty()) {
            std::cout << node->transactionPool.transactions.size() << " transactions" << std::endl;
            std::cout << "Forging new block..." << std::endl;
            node->forge();
        }
    }
}

void SocketCommunication::peerDiscovery() {
    for(;;) {
        std::string message = handshakeMessage();
        broadcastPeerDiscovery(message.c_str());
        sleep(10);
    }
}

void SocketCommunication::handshake(int sock) {
    std::string message = handshakeMessage();
    send_node_message(sock, message.c_str());
}

std::string SocketCommunication::handshakeMessage() {
    nlohmann::json j;
    std::string objectAsString;
    std::string messageType = "DISCOVERY";
    Message handshakeMsg(sc, messageType, peers);
    std::string jsonMessage = handshakeMsg.toJson();
    return jsonMessage;
}

void SocketCommunication::broadcastPeerDiscovery(const char *message) {
    //std::cout << "broadcast " << std::endl;
    auto j = nlohmann::json::parse(message + MESSAGELENGTH);
    if (j["Message"]["Peers"] != nullptr) {
        
        //std::cout << " Checking list of peers to broadcast to " << std::endl;
        std::vector<std::string> dest;
        dest.assign(std::begin(j["Message"]["Peers"]), std::end(j["Message"]["Peers"]));
 
        std::vector<std::string> peersPeerList {dest};
        for(auto &ipPortStr : peersPeerList) {
            char *token = strtok((char *)ipPortStr.c_str(), ":");

            std::vector<std::string> ipPortStrV;
            while (token != nullptr)
            {
                ipPortStrV.emplace_back(token);
                token = strtok(nullptr, ":");
            }
            
            auto num = (int)stol(ipPortStrV.at(1));
            //std::cout << ipPortStrV.at(0) << ":" << num << std::endl;

            int outgoingSocket = p2putils::setOutgoingNodeConnection(ipPortStrV.at(0), num);
            if (outgoingSocket == -1) {
                // ======================================
                // Socket connection to peer failed, 
                // remove peer from peers list.
                // ======================================
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

            SocketCommunication::outbound_node_connected(outgoingSocket);
            // peerThread.join();
            #ifndef _WIN32
            int returnVal = close(outgoingSocket);
            #else
            int returnVal = closesocket(outgoingSocket);
            #endif
            if (returnVal != 0) {
                #ifndef _WIN32
                std::cout << "close() error " << errno << std::endl;
                #else
                std::cout << "\nclose() error: " << WSAGetLastError() << std::endl;
                #endif
            }
        }
    }
}

void SocketCommunication::broadcast(const char *message) const {
    if (peers.empty()) {return;}
    auto fullJsonString = nlohmann::json::parse(message + MESSAGELENGTH);

    // std::cout << "Broadcasting to the network..." << std::endl;

    std::vector<std::string> tempPeers = peers;
    for(auto &ipPortStr : tempPeers) {
        char *token = strtok((char *)ipPortStr.c_str(), ":");

        std::vector<std::string> ipPortStrV;
        while (token != nullptr)
        {
            ipPortStrV.emplace_back(token);
            token = strtok(nullptr, ":");
        }
        
        auto num = (int)stol(ipPortStrV.at(1));
        //std::cout << ipPortStrV.at(0) << ":" << num << std::endl;

        int outgoingSocket = p2putils::setOutgoingNodeConnection(ipPortStrV.at(0), num);
        if (outgoingSocket == -1) {            
            return;
        }

        send_node_message(outgoingSocket, message);
        #ifndef _WIN32
        int returnVal = close(outgoingSocket);
        #else
        int returnVal = closesocket(outgoingSocket);
        #endif
        if (returnVal != 0) {
            #ifndef _WIN32
            std::cout << "close() error " << errno << std::endl;
            #else
            std::cout << "\nclose() error: " << WSAGetLastError() << std::endl;
            #endif
        }
    }
}

void SocketCommunication::peerDiscoveryHandleMessage(const char *message) {

    if (!message)
        return;

    //std::cout << "peerDiscoveryHandleMessage message is " << message << std::endl;

    auto j = nlohmann::json::parse(message);
    // write this method.
    SocketConnector peersSenderConnector(
        j["Message"]["SocketConnector"]["ip"], 
        int(j["Message"]["SocketConnector"]["port"])
    );
 
    bool newPeer = true;
    if (!peers.empty()) {
        for (auto const &v : peers) {
            if (v == peersSenderConnector.ip + ":" + std::to_string(peersSenderConnector.port)) {
                newPeer = false;
            } 
        }
    }

    if (newPeer) {
        std::string newPeerToAdd;
        std::cout << "Adding peer via peer discovery: " 
                  << peersSenderConnector.ip 
                  << ":" 
                  << peersSenderConnector.port << std::endl;
        newPeerToAdd = peersSenderConnector.ip + ":" + std::to_string(peersSenderConnector.port);
        peers.push_back(newPeerToAdd);
        
        // std::string thisNode = sc.ip + ":" + std::to_string(sc.port);
        // std::string masterNode = utils::get_master_node_ip();
        // std::string masterNode2 = ":" + std::to_string(utils::get_master_node_port());
        // masterNode += masterNode2;

        // if (thisNode != masterNode) {
        std::cout << "Requesting blockchain from network" << std::endl;
        node->requestChain();
        // }
    }

    if (j["Message"]["Peers"] != nullptr) {
        
        //std::cout << " Checking list of peers for new peer to add " << std::endl;
        std::vector<std::string> dest;
        dest.assign(std::begin(j["Message"]["Peers"]), std::end(j["Message"]["Peers"]));
 
        std::vector<std::string> peersPeerList {dest};

        for (auto const &peersPeer : peersPeerList) {
            bool peerKnown = false;
            for (auto const &p : peers) {
                if (peersPeer == p) {
                    peerKnown = true;
                }
            }

            if (!peerKnown && peersPeer != sc.ip + ":" + std::to_string(sc.port)) {
                char *token = strtok((char *)peersPeer.c_str(), ":");

                std::vector<std::string> tcpPair;
                while (token != nullptr)
                {
                    tcpPair.emplace_back(token);
                    token = strtok(nullptr, ":");
                }
                
                auto num = (int)stol(tcpPair.at(1));

                std::string peerToCheck = tcpPair.at(0) + ":" + std::to_string(num);

                // make sure new peer isn't an inactive peer before adding
                if (std::find(inactivePeers.begin(), 
                                inactivePeers.end(), peerToCheck) == inactivePeers.end())
                {    
                    std::cout << "Adding peer via peer discovery: " 
                          << tcpPair.at(0)
                          << ":"
                          << std::to_string(num) 
                          << std::endl;
                    std::string newPeerToAdd = tcpPair.at(0) + ":" + std::to_string(num);
                    peers.push_back(newPeerToAdd);
                    inactivePeers.clear();
                    
                    // ###
                    // std::string portString = std::to_string(node->p2p->sc.port);
                    // ofstream peerFile;
                    // std::string peerFileName = "known_peers_for_" + node->p2p->sc.ip + ":" + portString + ".txt";
                    // peerFile.open (peerFileName, std::ios_base::app);
                    // peerFile << newPeerToAdd << std::endl;
                    // peerFile.close();
                }
            }
        }
    } 
}