#include "Message.hpp"

Message::Message(SocketConnector *sc, 
                 std::string messageType,
                 std::vector<std::unordered_map<std::string, int>> *peers)
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
            for (auto p : peer) {
                peersToSerialize.push_back(
                    p.first + ":" + std::to_string(p.second)
                );
            }
        }
        j["Message"]["Peers"] = peersToSerialize;
    }
    return to_string(j);
}