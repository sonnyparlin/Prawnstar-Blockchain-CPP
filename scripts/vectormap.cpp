// Example program
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

int main()
{
    std::vector<std::unordered_map<std::string, int>> peers;
    std::unordered_map<std::string, int> myMap;
    myMap.insert(std::pair<std::string,int>("192.168.1.1", 10001));
    peers.push_back(myMap);
    myMap.clear();
    myMap.insert(std::pair<std::string,int>("192.168.1.1", 10002));
    peers.push_back(myMap);
    myMap.clear();
    
    // for (auto const& [key, val] : myMap)
    // {
    //     std::cout << key        // string (key)
    //             << ':'  
    //             << val        // string's value
    //             << std::endl;
        
    //     peers.push_back(pair<std::string,int>(key,val))
        
    // }

    for( auto const& peer : peers ) {
        for( auto const& p : peer ) {
            std::cout << p.first << ":" << p.second << std::endl;
        }
    }
}
