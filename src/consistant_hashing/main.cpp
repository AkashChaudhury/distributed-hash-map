#include <iostream>
#include <string>
#include "hash_ring_final.h"
#include <cstdlib> 
#include <vector>

int main() {
    ConsistentHashRing ring(5);

    ring.addNode("NodeA");
    ring.addNode("NodeB");
    ring.addNode("NodeC");
    ring.addNode("NodeD");

    ring.printRing();

    vector<string> keys = {"apple", "banana", "grape", "date", "sugar cane"};
    for (const auto& key : keys) {
        cout << "Key '" << key << "' is mapped to: " << ring.getNodeForKey(key) << "\n";
    }

    ring.removeNode("NodeA");

    cout << "\nAfter Delete";

    for (const auto& key : keys) {
        cout << "Key '" << key << "' is mapped to: " << ring.getNodeForKey(key) << "\n";
    }

    return 0;
}
