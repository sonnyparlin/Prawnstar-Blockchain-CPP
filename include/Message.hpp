#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include "SocketConnector.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

class Message {
public:
    SocketConnector sc; /*!< Socket connector */
    std::string messageType; /*!< Message type */
    std::vector<std::string> peers; /*!< List of peers to send message to if it's a broadcast message */
    std::string data; /*!< message data */
    
    Message(const SocketConnector &sc,
            std::string messageType,
            std::vector<std::string> peers);

    Message(std::string messageType,
            std::string data);
    ~Message();

    // Write a toJson method for this
    std::string toJson();
};

#endif // _MESSAGE_H_