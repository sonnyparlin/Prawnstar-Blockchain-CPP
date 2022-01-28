#include "SocketCommunication.hpp"
#include "Message.hpp"
#include <nlohmann/json.hpp>

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

void SocketCommunication::sendToNode(int sock, const char *message) {
    char buffer[1024] = {0};
    int reader;
    send(sock, message, strlen(message), 0);
    reader = read ( sock, buffer, 1024 );
    if (reader <= 0) {
        if (close(sock) == -1) {
            p2putils::logit("Close problems");
            std::cout << "errno: " << errno << std::endl;
        }
    } else
        std::cout << buffer << std::endl;
}

void SocketCommunication::handshake(int sock) {
    std::string message = handshakeMessage();
    sendToNode(sock, message.c_str());
    return;
}

std::string SocketCommunication::handshakeMessage() {
    std::string objectAsString;
    std::string messageType = "DISCOVERY";
    Message handshakeMsg(&sc, messageType, &peers);
    std::string jsonMessage = handshakeMsg.toJson();
    return jsonMessage.c_str();
}

void SocketCommunication::broadcast(const char *message) {
    for (auto const peer : peers) {
        int peerSock = p2putils::setOutgoingNodeConnection(peer.first);
        sendToNode(peerSock, message);
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
        std::cout << "Invalid ip address" << std::endl;
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
        std::cout << "main P2P Server" << std::endl;
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

        std::cout << "P2P Server: " 
            << argv[1] 
            << " is listening on PORT: " 
            << argv[2]
            << std::endl;

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
            outgoingSocket = p2putils::setOutgoingNodeConnection(argv[3]);
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

void SocketCommunication::startPeerDiscovery() {
    std::thread statusThread (&SocketCommunication::peerDiscoveryStatus, this);
    statusThread.detach();

    std::thread discoveryThread (&SocketCommunication::peerDiscovery, this);
    discoveryThread.detach();
}

void SocketCommunication::peerDiscoveryStatus() {
    for(;;) {
        std::cout << "status" << std::endl;
        sleep(10);
    }
}

void SocketCommunication::peerDiscovery() {
    for(;;) {
        std::cout << "discovery" << std::endl;
        sleep(10);
    }
}