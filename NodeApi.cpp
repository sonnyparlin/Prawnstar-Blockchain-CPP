#include "NodeApi.hpp"
#include "Wallet.hpp"

NodeApi::NodeApi(Node *node) {
    this->node = node;
}

NodeApi::~NodeApi() {
}

void NodeApi::start(std::string po) {
    int port = stol(po);

    crow::SimpleApp app;

    CROW_ROUTE(app, "/info")([](){
        return "This is a communication interface to a node's blockchain.";
    });

    CROW_ROUTE(app, "/blockchain")([&](){
        return node->blockchain->toJsonWebView();
    });

    CROW_ROUTE(app, "/transaction-pool")([&](){
        std::string returnValue = node->transactionPool.getPoolTransactionsJsonString();
        return returnValue;
    });

    CROW_ROUTE(app, "/wallet/<string>")([&](std::string address){
        double returnValue = node->accountModel->getBalance(address);
        nlohmann::json j;
        j["amount"] = std::to_string(returnValue);
        return crow::response(200, j.dump());
    });

    // ONLY ALLOW REQUESTS FROM THIS MACHINE
    CROW_ROUTE(app, "/transact")
    .methods("POST"_method)
    ([&](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x)
            return crow::response(crow::status::BAD_REQUEST); 

        std::string str = req.body;
        if (!x.has("transaction"))
            return crow::response("Missing transaction value");
        
        std::string sender = x["transaction"]["sender"].s(); // extract sender
        sender.erase(remove( sender.begin(), sender.end(), '\"' ),sender.end());

        std::string receiver = x["transaction"]["receiver"].s(); // extract receiver
        receiver.erase(remove( receiver.begin(), receiver.end(), '\"' ),receiver.end());

        std::string type = x["transaction"]["type"].s(); // extract type
        type.erase(remove( type.begin(), type.end(), '\"' ),type.end());

        double amount = x["transaction"]["amount"].d();

        if (node->accountModel->accountExists(sender)) {
            Wallet senderWallet(sender.c_str(), node);
            Transaction tx = senderWallet.createTransaction(receiver, amount, type);
            bool success = node->handleTransaction(tx);
            if (!success)
                return crow::response(500, "{\"message\":\"Failed to handle transaction\"}");
        } else {
            return crow::response(201, "{\"message\":\"Wallet not found\"}");
        }
        return crow::response(201, "{\"message\":\"Received transaction\"}");
    });

    app.loglevel(crow::LogLevel::Warning);
    app.port(port).multithreaded().run();
}
