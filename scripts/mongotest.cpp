#include <iostream>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>

int main() {
	mongocxx::instance inst{}; // This should be done only once.
	mongocxx::client client{mongocxx::uri {"mongodb://127.0.0.1:27017"}};
	mongocxx::database db = client["prawnstardb127001-10001"];
    mongocxx::collection coll = db["blocks"];

    db.drop();

    std::string jsonString = R"({"_id":"***genesis***hash***",
                                "blockCount":0,"forgerAddress":"",
                                "hash":"***genesis***hash***",
                                "lastHash":"***no***last***hash***",
                                "signature":"","timestamp":0,
                                "transactions":[]})";

    std::string jsonString2 = R"({"_id": "819c1913c9cb1ad45ce9dd975039843e918f1c064f3b0f29d13f5239e5ab128",
                                    "blockCount": 2,
                                    "forgerAddress": "pv171e654b041c51d978becc5cf65b0e2f48ff7",
                                    "hash": "819c1913c9cb1ad45ce9dd975039843e918f1c064f3b0f29d13f5239e5ab128",
                                    "lastHash": "5ae168d173556fa156db513732ab86f82ff952bc32a59aa1030edc0f556274c",
                                    "signature": "3046022100FC9982CD173B50812D1D51C74492324DA37DE47DA419E849A5D63577CE9AAE91022100D426A860A46F0D574B64E9E7966F20A693753CAFBC0CCD0BFB7E0DEE0D948B90",
                                    "timestamp": 1646374641,
                                    "transactions": [{"amount":500,
                                                        "id": "3f532a90-3032-4d19-967d-b22ba155dabc",
                                                        "receiverAddress": "pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7",
                                                        "senderAddress": "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac",
                                                        "signature": "304502202979D883088AE3371A22FC9F5231698B7E2BCCA458FE3E3B128992F606E91C7D02210090E1E1F141AD36079F3874A4D740AE12AEAE143014AE6ECED5C92CFE270E2CD6",
                                                        "timestamp": 1646374641,
                                                        "type": "EXCHANGE"}]})";

    bsoncxx::document::value doc = bsoncxx::from_json(jsonString);
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result;
    result = coll.insert_one(doc.view());
    if (!result) {
        std::cout << "error: " << std::endl;
        exit(0);
    }

    doc = bsoncxx::from_json(jsonString2);
    result = coll.insert_one(doc.view());
    if (!result) {
        std::cout << "error: " << std::endl;
        exit(0);
    }

    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result;
    maybe_result = coll.find_one(bsoncxx::builder::stream::document{}
                << "_id"
                << "819c1913c9cb1ad45ce9dd975039843e918f1c064f3b0f29d13f5239e5ab128"
                << bsoncxx::builder::stream::finalize);
    if(maybe_result) {
        std::cout << "\n\n" << bsoncxx::to_json(*maybe_result) << std::endl;
    }

    maybe_result = coll.find_one(bsoncxx::builder::stream::document{}
                                  << "_id"
                                  << "***genesis***hash***"
                                  << bsoncxx::builder::stream::finalize);
    if(maybe_result) {
        std::cout << "\n\n" <<  bsoncxx::to_json(*maybe_result) << "\n\n";
    }
}
