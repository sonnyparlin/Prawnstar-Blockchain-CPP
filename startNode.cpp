#include <iostream>
#include <typeinfo>
#include "Node.hpp"
#include "ProofOfStake.hpp"
#include "Lot.hpp"

/*! \mainpage Development documentation for the Prawnstar Blockchain.
 *
 * See the "Dependancies Needed and Startup Instructions" under the [Related Pages](pages.html) tab to get started.
 */
int startNode(int argc, char *argv[]) {
    Node *node = Node::createNode(argc, argv);
    node->startServers(argc, argv);
    delete node;
	return 0;
}