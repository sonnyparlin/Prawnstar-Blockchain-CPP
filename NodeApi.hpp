#ifndef _NODEAPI_H_
#define _NODEAPI_H_
#include <vector>
#include <string>
#include <iostream>
#include <crow.h>
#include "Node.hpp"

class NodeApi {
public:
    NodeApi(Node *node);
    ~NodeApi();

    Node *node;
    void start(std::string);
    int validateApiPort(const std::string po);
};

#endif // _NODEAPI_H_
