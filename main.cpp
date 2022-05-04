#include <iostream>
#include "Node.hpp"
#include "utils.hpp"

/*! \mainpage Development documentation for the Prawnstar Blockchain.
 *
 * See the "Dependancies Needed and Startup Instructions" under the [Related Pages](pages.html) tab to get started.
 */

int main(int argc, char *argv[]) {
    utils::startUtils();
    Node *node = Node::createNode(argv);
    node->startServers(argc, argv);
    delete node;
	return 0;
}