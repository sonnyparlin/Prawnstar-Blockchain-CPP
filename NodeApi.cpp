#include "NodeApi.hpp"
#include "Wallet.hpp"

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

    CROW_ROUTE(app, "/transaction-pool")([&](){
        return transactionPool.getPoolTransactionsJsonString();
    });

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

        if (accountModel.accountExists(sender)) {
            Wallet senderWallet(sender, accountModel);
            Transaction tx = senderWallet.createTransaction(receiver, amount, type);
            handleTransaction(tx);
        } else {
            Wallet senderWallet;
            senderWallet.address = sender;
            accountModel.addAccount(sender, senderWallet.walletPublicKey, senderWallet.walletPrivateKey);
            Transaction tx = senderWallet.createTransaction(receiver, amount, type);
            handleTransaction(tx);
        }
        return crow::response("{\"message\":\"Received transaction\"}");
    });

    app.port(port).run();
}
