#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include "SocketConnector.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

class Message {
public:
    SocketConnector &sc;
    std::string messageType;
    std::vector<std::string> &peers;
    
    Message(SocketConnector *sc, 
            std::string messageType, 
            std::vector<std::string> *peers);
    ~Message();

    // Write a toJson method for this
    std::string toJson();
};

#endif // _MESSAGE_H_