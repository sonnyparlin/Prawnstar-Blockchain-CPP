#include "Message.hpp"
#include <utility>

/*!
 *
 * @param sc
 * @param messageType
 * @param peers
 *
 * Constructor for broadcast messages
 */
Message::Message(const SocketConnector &sc,
                 std::string messageType,
                 std::vector<std::string> peers)
                 :sc(sc), messageType(std::move(messageType)), peers(std::move(peers)){}

/*!
 *
 * @param messageType
 * @param data
 *
 * Constructor for messages
 */
Message::Message(std::string messageType,
                 std::string data)
                 :messageType(std::move(messageType)), data(std::move(data)){}

Message::~Message()=default;

/*!
 *
 * @return std::string
 *
 * Json represented string of message.
 */
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
        for(auto const &peer : peers) {
            peersToSerialize.push_back(peer);
        }
        j["Message"]["Peers"] = peersToSerialize;
    }

    std::string strMsgBody = to_string(j);
    
    // append message header with length
    std::string strMsgLength = std::to_string (strMsgBody.length());

    // restrict the header length to 20
    switch(strMsgLength.length()) {
        case(1):
            strMsgLength = "0000000000000000000" + strMsgLength; break;
        case(2):
            strMsgLength = "000000000000000000" + strMsgLength; break;
        case(3):
            strMsgLength = "00000000000000000" + strMsgLength; break;
        case(4):
            strMsgLength = "0000000000000000" + strMsgLength; break;
        case(5):
            strMsgLength = "000000000000000" + strMsgLength; break;
        case(6):
            strMsgLength = "00000000000000" + strMsgLength; break;
        case(7):
            strMsgLength = "0000000000000" + strMsgLength; break;
        case(8):
            strMsgLength = "000000000000" + strMsgLength; break;
        case(9):
            strMsgLength = "00000000000" + strMsgLength; break;
        case(10):
            strMsgLength = "0000000000" + strMsgLength; break;
        case(11):
            strMsgLength = "000000000" + strMsgLength; break;
        case(12):
            strMsgLength = "00000000" + strMsgLength; break;
        case(13):
            strMsgLength = "0000000" + strMsgLength; break;
        case(14):
            strMsgLength = "000000" + strMsgLength; break;
        case(15):
            strMsgLength = "00000" + strMsgLength; break;
        case(16):
            strMsgLength = "0000" + strMsgLength; break;
        case(17):
            strMsgLength = "000" + strMsgLength; break;
        case(18):
            strMsgLength = "00" + strMsgLength; break;
        case(19):
            strMsgLength = "0" + strMsgLength; break;
        default:
            std::cout << "invalid message length" << std::endl; break;
    }
    
    // std::cout << "Message Header => " << strMsgLength << std::endl;
    strMsgBody = strMsgLength + strMsgBody;    

    // std::cout << strMsgLength << std::endl;
    return strMsgBody;
}