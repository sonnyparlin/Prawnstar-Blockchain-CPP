#include "Message.hpp"

Message::Message(SocketConnector *sc, 
                 std::string messageType,
                 std::vector<std::string> *peers)
                 :sc(*sc), messageType(messageType), peers(*peers){};
Message::~Message(){};

std::string Message::toJson() { 
    nlohmann::json j;
    j["Message"]["SocketConnector"]["ip"] = sc.ip;
    j["Message"]["SocketConnector"]["port"] = sc.port;
    j["Message"]["Type"] = messageType;
    if (peers.empty()) {
        j["Message"]["Peers"]=nullptr;
    } else {
        std::vector<std::string> peersToSerialize;
        for(auto peer : peers) {
            peersToSerialize.push_back(peer);
        }
        j["Message"]["Peers"] = peersToSerialize;
    }
    return to_string(j);
}