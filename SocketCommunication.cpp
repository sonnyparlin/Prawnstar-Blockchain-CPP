#include "SocketCommunication.hpp"

SocketCommunication::SocketCommunication() {
}

SocketCommunication::~SocketCommunication() {
}

void SocketCommunication::startSocketCommunication(int argc, char *argv[]) {
    
    if (p2pserver.processArgs(argc, argv) != 0)
        exit (1);

    createServer(argc, argv);
}

void SocketCommunication::inbound_node_connected(int sock) {
    std::cout << "inbound connection" << std::endl;
    handle_incoming_node_message(sock, "Hi, I'm the node you connected to.");
}

void SocketCommunication::outbound_node_connected(int sock) {
    std::cout << "outbound connection" << std::endl;
    handle_incoming_node_message(sock, "Hi, I'm the node who initialized the connection.");
}

void SocketCommunication::handle_incoming_node_message(int sock, const char *message) {
    char buffer[1024] = {0};
    int reader;
    //char const* message = "Inbound connection";
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

int SocketCommunication::initializeWithMasterNode(int sock) {
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
    //sleep(1);
    return 0;
}

int SocketCommunication::createServer ( int argc, char **argv )
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
            //initializeWithMasterNode(outgoingSocket);
            //std::cout << "Shaking hands with master node" << std::endl;
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