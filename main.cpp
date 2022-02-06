#include <iostream>
#include <typeinfo>
#include "Node.hpp"

int main(int argc, char *argv[]) {
    Node node(argc, argv);
    node.startP2P(argc, argv);
	return 0;
}