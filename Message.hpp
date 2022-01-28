#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include "SocketConnector.hpp"
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

class Message {
public:
    SocketConnector &sc;
    std::string messageType;
    std::unordered_map<char *, int> &peers;
    
    Message(SocketConnector *sc, 
            std::string messageType, 
            std::unordered_map<char *, int> *peers);
    ~Message();

    // Write a toJson method for this
    std::string toJson();
};

#endif // _MESSAGE_H_