#include "SocketCommunication.hpp"
#include "Message.hpp"
#include <nlohmann/json.hpp>
#include <typeinfo>
#include <algorithm>


SocketCommunication::SocketCommunication() {
}

SocketCommunication::~SocketCommunication() {
}

void SocketCommunication::startSocketCommunication(int argc, char *argv[]) {
    
    if (processArgs(argc, argv) != 0)
        exit (1);

    startP2POperations(argc, argv);
}

void SocketCommunication::inbound_node_connected(int sock) {
    handshake(sock);
}

void SocketCommunication::outbound_node_connected(int sock) {
    handshake(sock);
}

void SocketCommunication::node_message(int sock, const char *message) {
    const int BUFFER_SIZE = 2048;
    char buffer[BUFFER_SIZE] = {0};
    int reader;
    send(sock, message, strlen(message), 0);

    // set up loop read for large data dumps
    
    reader = read ( sock, buffer, BUFFER_SIZE -1 );
    if (reader <= 0) {
        if (close(sock) == -1) {
            p2putils::logit("Close problems");
            std::cout << "errno: " << errno << std::endl;
        }
    }
    auto j = nlohmann::json::parse(buffer);
    std::string messageType = j["Message"]["Type"];

    //std::cout << "Incoming Msg: " << buffer << std::endl;
    if (messageType == "DISCOVERY") {
        // create peerDiscoveryHandleMessage method
        peerDiscoveryHandleMessage(buffer);
        //close(sock);
    }
}

int SocketCommunication::processArgs(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "Usage for the first node: ./server <ip> <port>" << std::endl;
        return 1;
    }

    if (argc < 4) {
        std::cout << "Node usage for peers: ./server <ip> <port> <masternode>" << std::endl;
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
    
    for(;;) {
        // keep alive
        sleep(10);
    }
}

int SocketCommunication::startP2PServer ( int argc, char **argv )
{
    // Everything went well, set the port
    char *p;
    int PORT = strtol(argv[2], &p, 10);
    std::string id = utils::uuid_gen();
    struct sockaddr_in address;
    int address_length = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 

    // Server initialization
    int serverSocket = p2putils::createSocket();    
    if (!p2putils::Bind(serverSocket, address, PORT))
        exit (1);

    int i=0;
    std::cout << "Server created with id: " << id << std::endl;

    for (;;) {
        int outgoingSocket;
        int incomingSocket;

        if (PORT != 10001 && i !=0 ) {
            if ((incomingSocket = accept(serverSocket, 
                (struct sockaddr *)&address, 
                (socklen_t*)&address_length)) < 0) {
                    p2putils::logit("Accept");
                    exit(EXIT_FAILURE);
            }
        } else if (PORT == 10001) {
            if ((incomingSocket = accept(serverSocket, 
                (struct sockaddr *)&address, 
                (socklen_t*)&address_length)) < 0) {
                    p2putils::logit("Accept");
                    exit(EXIT_FAILURE);
            }
        }

        //=========================================================
        // Master node handshake
        //=========================================================   
        if (PORT != 10001 && i==0 && argc == 4) {
            outgoingSocket = p2putils::setOutgoingNodeConnection(argv[3], 10001);
            std::thread peerThread (&SocketCommunication::outbound_node_connected, this, outgoingSocket);
            peerThread.join();
        }

        //================================================================
        // Server operations
        //================================================================
        if (PORT != 10001 && i > 0) {
            std::thread peerThread (&SocketCommunication::inbound_node_connected, this, incomingSocket);
            peerThread.join();

        } else if (PORT == 10001) {
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
        if (!peers.empty()) {
            std::cout << "Current connections: " << std::endl;
            //std::cout << "There are " << peers.size() << " peers connected" << std::endl;
            for( auto const& peer : peers ) {
                std::cout << peer << std::endl;
            }
        }
        sleep(10);
    }
}

void SocketCommunication::peerDiscovery() {
    for(;;) {
        std::string message = handshakeMessage();
        //std::cout << "Broadcasting: " << message << std::endl;
        broadcast(message.c_str());
        sleep(10);
    }
}

void SocketCommunication::handshake(int sock) {
    std::string message = handshakeMessage();
    //send(sock, &message, message.length(), 0);
    node_message(sock, message.c_str());
    return;
}

std::string SocketCommunication::handshakeMessage() {
    nlohmann::json j;
    std::string objectAsString;
    std::string messageType = "DISCOVERY";
    Message handshakeMsg(&sc, messageType, &peers);
    std::string jsonMessage = handshakeMsg.toJson();
    return jsonMessage.c_str();
}

void SocketCommunication::broadcast(const char *message) {
    //std::cout << "broadcast " << std::endl;
    auto j = nlohmann::json::parse(message);
    if (j["Message"]["Peers"] != nullptr) {
        
        //std::cout << " Checking list of peers to broadcast to " << std::endl;
        std::vector<std::string> dest;
        dest.assign(std::begin(j["Message"]["Peers"]), std::end(j["Message"]["Peers"]));
 
        std::vector<std::string> peersPeerList {dest};
        for(auto &ipPortStr : peersPeerList) {
            char *token = strtok(ipPortStr.data(), ":");

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
                peers.erase(std::remove(peers.begin(), peers.end(), elementToRemove), peers.end());
                inactivePeers.push_back(elementToRemove);
                return;
            }
            std::thread peerThread (&SocketCommunication::outbound_node_connected, this, outgoingSocket);
            peerThread.join();
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
                char *token = strtok(peersPeer.data(), ":");

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
                    peers.push_back(tcpPair.at(0) + ":" + std::to_string(num));
                    inactivePeers.clear();
                }
            }
        }
    } 
}