#include "hash_ring_final.h"
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <set>
#include <thread>

void handleClient(int clientSock, HashTable* ht) {
    char buffer[4096] = {0};
    int bytesRead = recv(clientSock, buffer, sizeof(buffer), 0);
    if (bytesRead > 0) {
        std::string request(buffer);
        std::string response = ht->handleRPCRequest(request);
        send(clientSock, response.c_str(), response.length(), 0);
    } else {
        std::cerr << "Error receiving data from client." << std::endl;
    }
    close(clientSock);  // Close the client socket
}

void startHashTableServer(const std::string& node, map<size_t, string> hashes, std::set<std::string>& allNodes) {
    auto pos = node.find(":");
    std::string portStr = node.substr(pos + 1);
    int port = std::stoi(portStr);

    // Create the HashTable instance
    HashTable* ht = new HashTable(50, node, hashes, allNodes);

    // --- Set up server socket
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == -1) {
        std::cerr << "Error creating socket for " << node << std::endl;
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "Bind failed for " << node << " with error: " << strerror(errno) << std::endl;
        close(serverSock);
        return;
    }

    if (listen(serverSock, 10) == -1) {
        std::cerr << "Listen failed on " << node << std::endl;
        close(serverSock);
        return;
    }

    std::cout << "Hash table server listening on " << node << std::endl;

    // --- Accept loop
    while (true) {
        int clientSock = accept(serverSock, nullptr, nullptr);
        if (clientSock == -1) {
            std::cerr << "Failed to accept connection on " << node << std::endl;
            continue;
        }

        std::thread([clientSock, ht]() {
            handleClient(clientSock, ht);  // Handle the client in a new thread
        }).detach();
    }

    // Cleanup (though this will likely never be reached)
    close(serverSock);
    delete ht;
}


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

    // Start the hash table server in a separate thread
    thread serverThread(startHashTableServer, node, std::ref(this->hashRing), std::ref(this->nodes));  
    serverThread.detach(); 
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