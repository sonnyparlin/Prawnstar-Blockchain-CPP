#include "p2pserver.hpp"


P2PServer::P2PServer() {}
P2PServer::~P2PServer() {}

bool P2PServer::nodeInMapList(std::string ipAddress) {
    auto search = nodeMapList.find(ipAddress);
    if (search != nodeMapList.end()) {
        return true;
    } else {
        return false;
    }
}

std::unordered_map<std::string, int> P2PServer::getNodeMapList() {
    return nodeMapList;
}

int P2PServer::socketReceiveHandler(int s) {
    struct sockaddr_in addr;
    
    char buffer[1024] = {0};
    int reader;
    reader = read(s, buffer, 1024);
    if (reader <= 0)
        return 0;

    socklen_t len;
    len = sizeof(addr);
    getpeername(s, (struct sockaddr*)&addr, &len);
    std::stringstream ss;
    int PORT;
    ss << inet_ntoa(addr.sin_addr);
    PORT = ntohs(addr.sin_port);

    if (!nodeInMapList(ss.str()))
        nodeMapList[ss.str()]=PORT;

    // return the list to the client node

    std::cout << "Connection from: " 
              << ss.str()
              << " : " 
              << PORT << std::endl;

    char const* message = "I am an amazing server!";
    std::cout << "received: " << buffer << std::endl; 
    send(s, message, strlen(message) , 0 );
    printf("Server : Message has been sent ! \n");

    if (close(s) == -1) {
        p2putils::logit("Close problems");
        std::cout << "errno: " << errno << std::endl;
    }
    
    return 0;
}

int P2PServer::initializeWithMasterNode(int sock) {
    char buffer[1024] = {0};
    int reader;
    char const* message = "I am an amazing client.";
    send(sock, message, strlen(message) , 0 );
    printf("Client : Message has been sent ! \n");
    reader = read ( sock, buffer, 1024 );
    printf ( "%s\n",buffer );
    if (close(sock) == -1) {
        p2putils::logit("Close problems");
        std::cout << "errno: " << errno << std::endl;
    }
    sleep(1);
    return 0;
}

int P2PServer::processArgs(int argc, char **argv) {
    
    
    if (argc < 3) {
        std::cout << "Usage for the first node: ./p2pserver <ip> <port>" << std::endl;
        return 1;
    }

    if (argc < 4) {
        std::cout << "Node usage for peers: ./p2pserver <ip> <port> <masternode>" << std::endl;
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

int P2PServer::createServer ( int argc, char **argv )
{
    // Everything went well, set the port
    char *p;
    int PORT = strtol(argv[2], &p, 10);
    std::string id = p2putils::uuid_gen();
    struct sockaddr_in address;
    int address_length = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 

    // Server initialization
    int serverSocket = p2putils::createSocket();    
    if (!p2putils::Bind(serverSocket, address, PORT))
        exit (1);

    int STATE=0;
    if (PORT == 10001) STATE=1;
    int i=0;

    if (PORT != 10001 && i !=0 )
        std::cout << "Server created with id: " << id << std::endl;
    else if (PORT == 10001)
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
            struct sockaddr_in address;
            int address_length = sizeof(address);
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY; 
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
            initializeWithMasterNode(outgoingSocket);
            //std::cout << "Master node handshake" << std::endl;
            STATE=1;
        }

        //================================================================
        // Server operations
        //================================================================
        if (PORT != 10001 && STATE==1 && i > 0) {
            std::thread peerThread (&P2PServer::socketReceiveHandler, this, incomingSocket);
            peerThread.join();

        } else if (PORT == 10001) {
            std::thread peerThread (&P2PServer::socketReceiveHandler,this, incomingSocket);
            peerThread.join();
        }
        i++;
        //std::cout << i << std::endl;
    }
    return 0;
}