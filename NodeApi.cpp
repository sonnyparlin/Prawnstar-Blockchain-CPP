#include "NodeApi.hpp"

NodeApi::NodeApi() {
}

NodeApi::~NodeApi() {
}

void NodeApi::start(std::string po) {
    char* p;
    errno = 0; // not 'int errno', because the '#include' already defined it
    long arg = strtol(po.c_str(), &p, 10);

    if (*p != '\0' || errno != 0) {
        std::cout << "Invalid null or ernno not 0" << std::endl;
        return; // In main(), returning non-zero means failure
    }

    if (arg < INT_MIN || arg > INT_MAX) {
        std::cout << "Invalid port" << std::endl;
        return;
    }

    crow::SimpleApp app;

    CROW_ROUTE(app, "/info")([](){
        return "This is a communication interface to a node's blockchain.";
    });

    app.port(arg).run();
}
