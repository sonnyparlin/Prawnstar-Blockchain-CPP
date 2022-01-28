#include "Message.hpp"

Message::Message(SocketConnector *sc, 
                 std::string messageType,
                 std::unordered_map<char *, int> *peers)
                 :sc(*sc),messageType(messageType), peers(*peers){};
Message::~Message(){};

std::string Message::toJson() { 
    nlohmann::json j;
    j["Message"]["SocketConnector"]["ip"] = sc.ip;
    j["Message"]["SocketConnector"]["port"] = sc.port;
    j["Message"]["Type"] = messageType;
    for(auto peer : peers) {
        j["Message"]["Peers"]["ip"]=peer.first;
        j["Message"]["Peers"]["port"]=peer.second;
    }
    if (peers.empty()) {
        j["Message"]["Peers"]=nullptr;
    }
    return to_string(j);
}