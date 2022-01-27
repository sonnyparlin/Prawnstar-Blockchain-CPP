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