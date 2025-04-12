#include "hash_ring_final.h"
#include <iostream>

ConsistentHashRing::ConsistentHashRing(int virtualNodes) {
    numVirtualNodes = virtualNodes;
}

int ConsistentHashRing::hashFunc(const string& key) {
    return HashFunc(key, MAX_HASH);
}

string ConsistentHashRing::getNodeForKey (string key) {
    if (hashRing.empty()) return "";

    int hashVal = hashFunc(key);
    auto it = hashRing.lower_bound(hashVal);
    if (it == hashRing.end()) return hashRing.begin()->second;
    return it->second;
}

void ConsistentHashRing::addNode (const string& node) {
    if (nodes.count(node)) return;
    nodes.insert(node);

    for (int i = 0; i < numVirtualNodes; ++i) {
        string virtualNode = node + "#VN" + to_string(i);
        int hashVal = hashFunc(virtualNode);
        hashRing[hashVal] = node;
    }
}

void ConsistentHashRing::removeNode (const string& node)  {
    if (!nodes.count(node)) return;
    nodes.erase(node);

    for (int i = 0; i < numVirtualNodes; ++i) {
        string virtualNode = node + "#VN" + to_string(i);
        int hashVal = hashFunc(virtualNode);
        hashRing.erase(hashVal);
    }
}

