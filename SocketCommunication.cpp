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

SocketCommunication::~SocketCommunication() {
}

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
    int result = send(sock, message, strlen(message), 0);
    if (result < 0) {
        #ifndef _WIN32
        std::cout << "send() error " << errno << std::endl;
        #else
        printf("\nsend() error: %d\n", WSAGetLastError());
        #endif
        return;
    }
}

void SocketCommunication::receive_node_message(int sock) {
    int reader;

    // read message body length
    unsigned long long msgLength;
    char msgLengthBuffer[MESSAGELENGTH];
    
    reader = recv (sock, msgLengthBuffer, MESSAGELENGTH, MSG_WAITALL);
    if (reader < 0) {
        #ifndef _WIN32
        std::cout << "read() error " << errno << std::endl;
        #else
        printf("\nread() error: %d\n", WSAGetLastError());
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
        
        Transaction tx;
        tx.id = jx["id"];
        tx.amount = jx["amount"];
        tx.senderAddress = jx["senderAddress"];
        tx.senderPublicKey = jx["senderPublicKey"];
        tx.receiverAddress = jx["receiverAddress"];
        tx.signature = jx["signature"];
        tx.timestamp = jx["timestamp"];
        tx.type = jx["type"];

        node->handleTransaction(tx, false);
    } else if (messageType == "BLOCK") {
        Block block;
        std::string jsonBlock = j["Message"]["data"];
        nlohmann::json jx;

        try {
            jx = nlohmann::json::parse(jsonBlock);
        } catch(exception &e) {
            std::cerr << e.what() << std::endl;
        }
        
        for (auto tx : jx["transactions"]) {            
            Transaction tr;
            tr.id = tx["id"];
            tr.amount = tx["amount"];
            tr.senderAddress = tx["senderAddress"];
            Wallet senderWallet(tr.senderAddress.c_str(), this->node);
            tr.senderPublicKey = senderWallet.walletPublicKey;
            tr.receiverAddress = tx["receiverAddress"];
            tr.signature = tx["signature"];
            tr.timestamp = tx["timestamp"];
            tr.type = tx["type"];
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
        printf("\nclose() error: %d\n", WSAGetLastError());
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
    
    char* p;
    errno = 0; // not 'int errno', because the '#include' already defined it
    long arg = strtol(argv[2], &p, 10);

    if (*p != '\0' || errno != 0) {
        std::cout << "Invalid null or ernno not 0" << std::endl;
        return 1; // In main(), returning non-zero means failure
    }

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
    char *p;
    sc.port = strtol(argv[2], &p, 10);

    std::thread serverThread (&SocketCommunication::startP2PServer, this, argc, argv);
    serverThread.detach();

    std::thread statusThread (&SocketCommunication::peerDiscoveryStatus, this);
    statusThread.detach();

    std::thread discoveryThread (&SocketCommunication::peerDiscovery, this);
    discoveryThread.detach();
}

int SocketCommunication::startP2PServer ( int argc, char **argv )
{
    // Everything went well, set the port
    char *p;
    int PORT = strtol(argv[2], &p, 10);
    id = utils::get_uuid();
    struct sockaddr_in address;
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

        if (PORT != utils::MASTER_NODE_PORT && i !=0 ) {
            if ((incomingSocket = accept(serverSocket, 
                (struct sockaddr *)&address, 
                (socklen_t*)&address_length)) < 0) {
                    #ifndef _WIN32
                    std::cout << "accept() error " << errno << std::endl;
                    #else
                    printf("\naccept() error: %d\n", WSAGetLastError());
                    #endif
                    exit(0);
            }
        } else if (PORT == utils::MASTER_NODE_PORT) {
            if ((incomingSocket = accept(serverSocket, 
                (struct sockaddr *)&address, 
                (socklen_t*)&address_length)) < 0) {
                    #ifndef _WIN32
                    std::cout << "accept() error " << errno << std::endl;
                    #else
                    printf("\naccept() error: %d\n", WSAGetLastError());
                    #endif
                    exit(0);
            }
        }

        //=========================================================
        // Master node handshake
        //=========================================================   
        if (PORT != utils::MASTER_NODE_PORT && i==0 && argc == 5) {
            outgoingSocket = p2putils::setOutgoingNodeConnection(argv[3], utils::MASTER_NODE_PORT);
            std::thread peerThread (&SocketCommunication::outbound_node_connected, this, outgoingSocket);
            peerThread.join();
        }

        //================================================================
        // Server operations
        //================================================================
        if (PORT != utils::MASTER_NODE_PORT && i > 0) {
            std::thread peerThread (&SocketCommunication::inbound_node_connected, this, incomingSocket);
            peerThread.join();
        } else if (PORT == utils::MASTER_NODE_PORT) {
            std::thread peerThread (&SocketCommunication::inbound_node_connected,this, incomingSocket);
            peerThread.join();
        }
        i++;
        //std::cout << i << std::endl;
    }
    return 0;
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
    return;
}

std::string SocketCommunication::handshakeMessage() {
    nlohmann::json j;
    std::string objectAsString;
    std::string messageType = "DISCOVERY";
    Message handshakeMsg(sc, messageType, peers);
    std::string jsonMessage = handshakeMsg.toJson();
    return jsonMessage.c_str();
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
            while (token != NULL)
            {
                ipPortStrV.push_back(token);
                token = strtok(NULL, ":");
            }
            
            int num = atoi(ipPortStrV.at(1).c_str());
            //std::cout << ipPortStrV.at(0) << ":" << num << std::endl;

            int outgoingSocket = p2putils::setOutgoingNodeConnection(ipPortStrV.at(0), num);
            if (outgoingSocket == -1) {
                // ======================================
                // Socket connection to peer failed, 
                // remove peer from peers list.
                // ======================================
                std::string elementToRemove = ipPortStrV.at(0) + ":" + std::to_string(num);
                std::string masterNode = utils::MASTER_NODE_IP;
                std::string masterNode2 = ":" + std::to_string(utils::MASTER_NODE_PORT);
                masterNode += masterNode2;

                if (elementToRemove != masterNode) {
                    peers.erase(std::remove(peers.begin(), peers.end(), elementToRemove), peers.end());
                    inactivePeers.push_back(elementToRemove);
                }
                return;
            }
            std::thread peerThread (&SocketCommunication::outbound_node_connected, this, outgoingSocket);
            peerThread.join();
        }
    }
}

void SocketCommunication::broadcast(const char *message) {
    if (peers.empty()) {return;}
    auto fullJsonString = nlohmann::json::parse(message + MESSAGELENGTH);

    // std::cout << "Broadcasting to the network..." << std::endl;

    std::vector<std::string> tempPeers = peers;
    for(auto &ipPortStr : tempPeers) {
        char *token = strtok((char *)ipPortStr.c_str(), ":");

        std::vector<std::string> ipPortStrV;
        while (token != NULL)
        {
            ipPortStrV.push_back(token);
            token = strtok(NULL, ":");
        }
        
        int num = atoi(ipPortStrV.at(1).c_str());
        //std::cout << ipPortStrV.at(0) << ":" << num << std::endl;

        int outgoingSocket = p2putils::setOutgoingNodeConnection(ipPortStrV.at(0), num);
        if (outgoingSocket == -1) {            
            return;
        }
        
        // send_node_message(outgoingSocket, message);

        std::thread broadcastThread (&SocketCommunication::send_node_message, this, outgoingSocket, message);
        broadcastThread.join();
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
        for (auto v : peers) {
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
        // std::string masterNode = utils::MASTER_NODE_IP;
        // std::string masterNode2 = ":" + std::to_string(utils::MASTER_NODE_PORT);
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

        for (auto peersPeer : peersPeerList) {
            bool peerKnown = false;
            for (auto p : peers) {
                if (peersPeer == p) {
                    peerKnown = true;
                }
            }

            if (!peerKnown && peersPeer != sc.ip + ":" + std::to_string(sc.port)) {
                char *token = strtok((char *)peersPeer.c_str(), ":");

                std::vector<std::string> tcpPair;
                while (token != NULL)
                {
                    tcpPair.push_back(token);
                    token = strtok(NULL, ":");
                }
                
                int num = atoi(tcpPair.at(1).c_str());

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