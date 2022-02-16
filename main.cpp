#include <iostream>
#include <typeinfo>
#include "Node.hpp"
#include "ProofOfStake.hpp"
#include "Lot.hpp"

int main(int argc, char *argv[]) {
    Node *node = Node::createNode(argc, argv);
    node->startServers(argc, argv);
    delete node;
	return 0;
}