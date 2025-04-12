#ifndef HASH_RING_FINAL_H
#define HASH_RING_FINAL_H

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "../hash_functions/hash_functions.h"
#include "../hashtable/hash.h"
using namespace std;


void startHashTableServer(const string& node, map<size_t, string> hashes, set<string>& allNodes, map<string, HashTable*> &serverMap);
void handleClient(int clientSock, HashTable* ht);

class ConsistentHashRing {
    int numVirtualNodes;
    map<size_t, string> hashRing;
    set<string> nodes;
    map<string, HashTable*> serverMap;
    // void handleClient(int clientSock, HashTable* ht);

public:
    ConsistentHashRing(int virtualNodes);
    void addNode(const string& node);
    void removeNode(const string& node);
    string getNodeForKey(string key);
    int hashFunc(const string& key);
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