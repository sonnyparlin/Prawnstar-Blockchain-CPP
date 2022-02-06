#ifndef _NODEAPI_H_
#define _NODEAPI_H_
#include <vector>
#include <string>
#include <iostream>
#include <crow.h>

class NodeApi {
public:
    NodeApi();
    ~NodeApi();

    void start(std::string);
    int validateApiPort(const std::string po);
};

#endif // _NODEAPI_H_
