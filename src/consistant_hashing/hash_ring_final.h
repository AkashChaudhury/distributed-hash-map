#ifndef HASH_RING_FINAL_H
#define HASH_RING_FINAL_H

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

static const size_t MAX_HASH = 1e6;

class ConsistentHashRing {
    int numVirtualNodes;
    map<size_t, string> hashRing;
    set<string> nodes;

    int hashFunc(const string& key);

public:
    ConsistentHashRing(int virtualNodes);
    void addNode(const string& node);
    void removeNode(const string& node);
    string getNodeForKey(string key);
    void printRing() const {
        cout << "=== Hash Ring State ===\n";
        for (auto& node: hashRing) {
            cout << node.first << " => " << node.second << "\n";
        }
        cout << "========================\n";
    }

    void printNodes() const {
        cout << "Active nodes:\n";
        for (auto& node : nodes) {
            cout << node << "\n";
        }
    }
};

#endif