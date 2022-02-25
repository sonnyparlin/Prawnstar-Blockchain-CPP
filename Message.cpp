#include "Message.hpp"

Message::Message(SocketConnector sc, 
                 std::string messageType,
                 std::vector<std::string> peers)
                 :sc(sc), messageType(messageType), peers(peers){};

Message::Message(std::string messageType,
                 std::string data) // refactor to be a string
                 :messageType(messageType), data(data){};

Message::~Message(){};

std::string Message::toJson() { 
    nlohmann::json j;
    j["Message"]["SocketConnector"]["ip"] = sc.ip;
    j["Message"]["SocketConnector"]["port"] = sc.port;
    j["Message"]["Type"] = messageType;


    if (peers.empty()) {
        j["Message"]["Peers"]=nullptr;
        j["Message"]["data"]=data;
    } else {
        std::vector<std::string> peersToSerialize;
        for(auto peer : peers) {
            peersToSerialize.push_back(peer);
        }
        j["Message"]["Peers"] = peersToSerialize;
    }

    std::string strMsgBody = to_string(j);
    
    // append message header with length
    std::string strMsgLength = std::to_string (strMsgBody.length());

    // restrict the header length to 5
    if (strMsgLength.length() == 1) strMsgLength = "0000" + strMsgLength;    
    else if (strMsgLength.length() == 2) strMsgLength = "000" + strMsgLength;
    else if (strMsgLength.length() == 3) strMsgLength = "00" + strMsgLength;
    else if (strMsgLength.length() == 4) strMsgLength = "0" + strMsgLength;
    
    // std::cout << "Message Header => " << strMsgLength << std::endl;
    strMsgBody = strMsgLength + strMsgBody;    

    return strMsgBody;
}