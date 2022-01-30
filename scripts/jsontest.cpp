#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <nlohmann/json.hpp>

int main() {
    // create an empty structure (null)
    nlohmann::json j;

    // add a number that is stored as double (note the implicit conversion of j to an object)
    j["pi"] = 3.141;

    // add a Boolean that is stored as bool
    j["happy"] = true;

    // add a string that is stored as std::string
    j["name"] = "Niels";

    // add another null object by passing nullptr
    j["nothing"] = nullptr;

    // add an object inside the object
    j["answer"]["everything"] = 42;

    // add an array that is stored as std::vector (using an initializer list)
    j["list"] = { 1, 0, 2 };

    // add another object (using an initializer list of pairs)
    j["object"]["Peers"] = { { "192.168.1.1", 10001 },
                             { "192.168.1.2", 10002 } };

    std::cout << j.dump(4) << std::endl;

    return 0;
}
