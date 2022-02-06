#include "NodeApi.hpp"

NodeApi::NodeApi() {
}

NodeApi::~NodeApi() {
}

int NodeApi::validateApiPort(const std::string po) {
    char* p;
    errno = 0; // not 'int errno', because the '#include' already defined it
    long arg = strtol(po.c_str(), &p, 10);

    if (*p != '\0' || errno != 0) {
        std::cout << "Invalid null or ernno not 0" << std::endl;
        return -1;
    }

    if (arg < INT_MIN || arg > INT_MAX) {
        std::cout << "Invalid port" << std::endl;
        return -1;
    }

    return arg;
}

void NodeApi::start(std::string po) {
    int port = validateApiPort(po);

    crow::SimpleApp app;

    CROW_ROUTE(app, "/info")([](){
        return "This is a communication interface to a node's blockchain.";
    });

    CROW_ROUTE(app, "/blockchain")([&](){
        return blockchain.toJsonString();
    });

    app.port(port).run();
}
