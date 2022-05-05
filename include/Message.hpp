#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include "SocketConnector.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

/*!
 * Message class for sending messages on the peer to peer network.
 */
class Message {
public:
    Message(const SocketConnector &sc,
            std::string messageType,
            std::vector<std::string> peers);

    Message(std::string messageType,
            std::string data);
    ~Message();

    std::string toJson();

    SocketConnector sc; /*!< Socket connector */
    std::string messageType; /*!< Message type */
    std::vector<std::string> peers; /*!< List of peers to send message to if it's a broadcast message */
    std::string data; /*!< message data */
};

#endif // _MESSAGE_H_