#include <iostream>
#include <typeinfo>
#include "Node.hpp"

int main() {
    Node node;
    std::cout << typeid(node.blockchain).name() << std::endl;
    std::cout << typeid(node.transactionPool).name() << std::endl;
    std::cout << typeid(node.wallet).name() << std::endl;
    
	return 0;
}