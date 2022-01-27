#include "SocketCommunication.hpp"

PeerDiscoveryHandler::PeerDiscoveryHandler(SocketCommunication *sc) {
}

PeerDiscoveryHandler::~PeerDiscoveryHandler() {
}

void PeerDiscoveryHandler::start() {
    std::thread statusThread (&PeerDiscoveryHandler::status, this);
    statusThread.detach();

    std::thread discoveryThread (&PeerDiscoveryHandler::discovery, this);
    discoveryThread.detach();
}

void PeerDiscoveryHandler::status() {
    for(;;) {
        std::cout << "status" << std::endl;
        sleep(10);
    }
}

void PeerDiscoveryHandler::discovery() {
    for(;;) {
        std::cout << "discovery" << std::endl;
        sleep(10);
    }
}