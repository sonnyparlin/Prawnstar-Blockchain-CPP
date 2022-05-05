#ifndef _NODEAPI_H_
#define _NODEAPI_H_
#include <vector>
#include <string>
#include <iostream>
#include <crow.h>
#include "Node.hpp"

class Node;

/*!
 * The API class
 */
class NodeApi {
private:
    Node *node {};
public:
    explicit NodeApi(Node *node);
    ~NodeApi();
    
    void start(std::string&);
};

#endif // _NODEAPI_H_
