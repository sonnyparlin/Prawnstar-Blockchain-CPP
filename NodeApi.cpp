#include "NodeApi.hpp"
#include "Wallet.hpp"

NodeApi::NodeApi(Node *node) {
    this->node = node;
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
        return node->blockchain.toJsonString();
    });

    CROW_ROUTE(app, "/transaction-pool")([&](){
        //std::cout << "This is a transaction pool" << std::endl;
        std::string returnValue = node->transactionPool.getPoolTransactionsJsonString();
        //std::cout << "transactionPool contents: " << node->transactionPool.getPoolTransactionsJsonString() << std::endl;
        return returnValue;
    });

    CROW_ROUTE(app, "/clear-transaction-pool")([&](){
        node->transactionPool.transactions.clear();
        return crow::response(200, "{\"message\":\"cleared transaction pool\"}");
    });

    // CROW_ROUTE(app, "/transaction-broadcast")
    // .methods("POST"_method)
    // ([&](const crow::request& req) {
    //     std::string str = req.body;
    //     auto x = crow::json::load(req.body);
    //     std::cout << "x: " << x << std::endl;
    //     //
    //     if (!x)
    //         return crow::response(crow::status::BAD_REQUEST);
    //     //
    //     if (!x.has("transaction"))
    //         return crow::response("Missing transaction value");
    //     //
    //     std::ostringstream req_stream; 
    //     // osstringstream for copying json elements
    //     //
    //     req_stream << x["transaction"]["id"]; // extract sender
    //     std::string id = req_stream.str(); // copy string
    //     id.erase(remove( id.begin(), id.end(), '\"' ),id.end());
    //     req_stream.str(std::string()); // clear the osstringstream
    //     //
    //     req_stream << x["transaction"]["senderAddress"]; // extract sender
    //     std::string sender = req_stream.str(); // copy string
    //     sender.erase(remove( sender.begin(), sender.end(), '\"' ),sender.end());
    //     req_stream.str(std::string()); // clear the osstringstream
    //     //
    //     req_stream << x["transaction"]["senderPublicKey"]; // extract sender
    //     std::string senderPublicKey = req_stream.str(); // copy string
    //     senderPublicKey.erase(remove( senderPublicKey.begin(), senderPublicKey.end(), '\"' ),senderPublicKey.end());
    //     req_stream.str(std::string()); // clear the osstringstream
    //     //
    //     req_stream << x["transaction"]["receiverAddress"]; // extract receiver
    //     std::string receiver = req_stream.str(); // copy string
    //     receiver.erase(remove( receiver.begin(), receiver.end(), '\"' ),receiver.end());
    //     req_stream.str(std::string()); // clear the osstringstream
    //     //
    //     req_stream << x["transaction"]["signature"]; // extract receiver
    //     std::string signature = req_stream.str(); // copy string
    //     signature.erase(remove( signature.begin(), signature.end(), '\"' ),signature.end());
    //     req_stream.str(std::string()); // clear the osstringstream
    //     //
    //     req_stream << x["transaction"]["timestamp"]; // extract receiver
    //     std::string timestamp = req_stream.str(); // copy string
    //     timestamp.erase(remove( timestamp.begin(), timestamp.end(), '\"' ),timestamp.end());
    //     req_stream.str(std::string()); // clear the osstringstream
    //     //
    //     req_stream << x["transaction"]["type"]; // extract type
    //     std::string type = req_stream.str(); // copy string
    //     type.erase(remove( type.begin(), type.end(), '\"' ),type.end());
    //     req_stream.str(std::string()); // clear the osstringstream
    //     //
    //     double amount = x["transaction"]["amount"].d();
    //     nlohmann::json j = nlohmann::json::parse(str);
    //     //
    //     Transaction tx;
    //     tx.id = id;
    //     tx.amount = amount;
    //     tx.senderAddress = sender;
    //     tx.senderPublicKey = senderPublicKey;
    //     tx.receiverAddress = receiver;
    //     tx.signature = signature;
    //     tx.timestamp = atol(timestamp.c_str());
    //     tx.type = type;
    //     //
    //     std::cout << "Calling handleTransaction(tx) from POST" << std::endl;
    //     handleTransaction(tx);
    //     return crow::response(201, "{\"message\":\"Transaction Broadcast\"}");
    // });

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

        // if (accountModel.accountExists(sender)) {
        //     Wallet senderWallet(sender, accountModel);
        //     Transaction tx = senderWallet.createTransaction(receiver, amount, type);
        //     handleTransaction(tx);
        // } else {
        Wallet senderWallet;
        senderWallet.address = sender;
        node->accountModel.addAccount(sender, senderWallet.walletPublicKey, senderWallet.walletPrivateKey);
        Transaction tx = senderWallet.createTransaction(receiver, amount, type);
        //std::cout << "Calling handleTransaction(tx) from POST" << std::endl;
        node->handleTransaction(tx);
        // }
        return crow::response(201, "{\"message\":\"Received transaction\"}");
    });

    //app.port(port).loglevel(crow::LogLevel::Warning).run();
    app.port(port).run();
}
