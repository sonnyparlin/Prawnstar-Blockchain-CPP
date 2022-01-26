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