#include <iostream>
#include <unordered_map>
#include <string>

class ComplexObject {
    public:
        int i{10};
        std::string s{"sonny"};
};

int main()
{
    ComplexObject co;
    ComplexObject co2;
    ComplexObject co3;
    ComplexObject co4;
    
    // Create an empty unordered_map
    std::unordered_map<std::string, ComplexObject> wordMap;
    // Insert Few elements in map
    wordMap.insert( { "addressxyz1", co });
    wordMap.insert(    { "addressxyz2", co2 });
    wordMap.insert(    { "addressxyz3", co3 });
    // Overwrite value of an element
    wordMap["addressxyz4"] = co4;
    // Iterate Over the unordered_map and display elements
    for (std::pair<std::string, ComplexObject> element : wordMap)
        std::cout << element.first << " :: " << element.second.i << std::endl;
    return 0;
}
