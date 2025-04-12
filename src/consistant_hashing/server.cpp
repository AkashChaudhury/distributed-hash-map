#include <iostream>
#include <string>
#include <set>
#include "hash_ring_final.h"  // Assuming this includes your ConsistentHashRing class

int main() {
    int virtualNodeCount = 3;  // Number of virtual nodes per real node
    ConsistentHashRing ring(virtualNodeCount);

    std::string input;
    while (true) {
        std::cout << "Enter a node address (localhost:<port>) to add or type 'done' to finish: ";
        std::getline(std::cin, input);

        if (input == "done") break;

        ring.addNode(input);
        std::cout << "Node " << input << " added successfully." << std::endl;
    }

    std::cout << "Hash table servers are now running." << std::endl;

    // Keep main alive so server threads keep running
    while (true) {
        sleep(10);
    }

    return 0;
}
