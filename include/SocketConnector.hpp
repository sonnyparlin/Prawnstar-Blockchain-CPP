#ifndef _SOCKETCONNECTOR_H_
#define _SOCKETCONNECTOR_H_
#include <iostream>
#include <string>

/*!
 * SocketConnector class
 * Just the IP and Port of this node.
 */
class SocketConnector {
public:
    SocketConnector(std::string, int);
    SocketConnector();
    ~SocketConnector();

    std::string ip{}; /*!< Ip address */
    int port{}; /*!< Port */
};

#endif // _SOCKETCONNECTOR_H_