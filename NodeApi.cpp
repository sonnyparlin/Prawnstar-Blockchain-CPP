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
            // same as crow::response(400)

        std::string str = req.body;
        if (!x.has("transaction"))
            return crow::response("Missing transaction value");
        
        std::ostringstream req_stream; 
        // osstringstream for copying json elements

        req_stream << x["transaction"]["sender"]; // extract sender
        std::string sender = req_stream.str(); // copy string
        sender.erase(remove( sender.begin(), sender.end(), '\"' ),sender.end());
        req_stream.str(std::string()); // clear the osstringstream
        
        req_stream << x["transaction"]["receiver"]; // extract receiver
        std::string receiver = req_stream.str(); // copy string
        receiver.erase(remove( receiver.begin(), receiver.end(), '\"' ),receiver.end());
        req_stream.str(std::string()); // clear the osstringstream

        req_stream << x["transaction"]["type"]; // extract type
        std::string type = req_stream.str(); // copy string
        type.erase(remove( type.begin(), type.end(), '\"' ),type.end());
        req_stream.str(std::string()); // clear the osstringstream

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

    app.port(port).run();
}
