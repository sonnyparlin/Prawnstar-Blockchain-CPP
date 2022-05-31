#include "NodeApi.hpp"
#include "Wallet.hpp"

/*!
 *
 * @param node
 *
 * Constructor for NodeApi
 */
NodeApi::NodeApi(Node *node) {
    this->node = node;
}

NodeApi::~NodeApi()=default;

/*!
 *
 * @param _port
 *
 * Starts the Node APU on port _port and contains all the routes for the APU.
 */
void NodeApi::start(std::string &_port) {
    long port = stol(_port);

    crow::SimpleApp app;

    CROW_ROUTE(app, "/info")([](){
        return crow::response(200, "Prawnstar Blockchain");
    });

    CROW_ROUTE(app, "/node")([&](){
        nlohmann::json j;
        j["message"] = node->getNodeID();
        std::vector<std::string> ip_port = utils::get_ip_and_port_from_config();
        j["ip"] = ip_port.at(0);
        j["port"] = ip_port.at(1);
        j["master"] = utils::get_master_node_ip();
        return crow::response(200, j.dump());
    });

    CROW_ROUTE(app, "/console")([&](){
        nlohmann::json j;
        j["console"] = Node::getConsoleLog();
        return crow::response(200, j.dump());
    });

    CROW_ROUTE(app, "/blockchain")([&](){
        return node->blockchain->toJson();
    });

    CROW_ROUTE(app, "/transaction-pool")([&](){
        std::string returnValue = node->blockchain->transactionPool.getPoolTransactionsJsonString();
        return crow::response(200, returnValue);
    });

    CROW_ROUTE(app, "/tx/<string>")([&](const std::string &txid){
        std::string jsonValue = node->blockchain->getTransaction(txid);
        nlohmann::json j = nlohmann::json::parse(jsonValue);
        return crow::response(200, j.dump());
    });

    CROW_ROUTE(app, "/wallet/<string>")([&](const std::string &address){
        double returnValue = node->blockchain->accountModel.getBalance(address);
        nlohmann::json j;
        j["amount"] = std::to_string(returnValue);
        return crow::response(200, j.dump());
    });

    CROW_ROUTE(app, "/nodewallet")([&](){
        double returnValue = node->blockchain->accountModel.getBalance(node->nodeWallet->address);
        nlohmann::json j;
        j["amount"] = std::to_string(returnValue);
        j["address"] = node->nodeWallet->address;
        j["publickey"] = node->nodeWallet->walletPublicKey;
        j["nodestake"] = node->proofOfStake->getStake(node->nodeWallet->walletPublicKey);
        return crow::response(200, j.dump());
    });

    CROW_ROUTE(app, "/nodetransactions")([&](){
        std::vector<nlohmann::json> txids = node->blockchain->txsByAddress(node->nodeWallet->address);
        std::reverse(txids.begin(), txids.end());
        
        nlohmann::json j;
        j = nlohmann::json::array({txids});
        return crow::response(200, j[0].dump());
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

        if (node->blockchain->accountModel.accountExists(sender)) {
            Wallet senderWallet(sender.c_str(), node);
            Transaction tx = senderWallet.createTransaction(receiver, amount, type);
            bool success = node->handleTransaction(tx);
            if (!success)
                return crow::response(500, R"({"message":"Failed to handle transaction"})");
        } else {
            return crow::response(201, R"({"message":"Wallet not found"})");
        }
        return crow::response(201, R"({"message":"Received transaction"})");
    });

    app.loglevel(crow::LogLevel::Warning);
    app.port(port).multithreaded().run();
}
