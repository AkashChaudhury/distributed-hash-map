#include "hash_ring_final.h"
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <set>
// wanted to use pthread, but using thread felt more easy
// thread can afford more args
#include <thread>

void handleClient(int clientSock, HashTable* ht) {
    char buffer[4096] = {0};
    int bytesRead = recv(clientSock, buffer, sizeof(buffer), 0);
    if (bytesRead > 0) {
        string request(buffer);
        string response = ht->handleRPCRequest(request);
        send(clientSock, response.c_str(), response.length(), 0);
    } else {
        cerr << "Error receiving data from client." << endl;
    }
    close(clientSock);  // Close the client socket
}

void startHashTableServer(const string& node, map<size_t, string> hashes, set<string>& allNodes, map<string, HashTable*> &serverMap) {
    auto pos = node.find(":");
    string portStr = node.substr(pos + 1);
    int port = stoi(portStr);

    HashTable* ht = new HashTable(50, node, hashes, allNodes);
    serverMap[node] = ht;

    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == -1) {
        cerr << "Error creating socket for " << node << endl;
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (sockaddr*)&addr, sizeof(addr)) == -1) {
        cerr << "Bind failed for " << node << " with error: " << strerror(errno) << endl;
        close(serverSock);
        return;
    }

    if (listen(serverSock, 10) == -1) {
        cerr << "Listen failed on " << node << endl;
        close(serverSock);
        return;
    }

    cout << "Hash table server listening on " << node << endl;

    while (!ht->serverStopSig()) {
        int clientSock = accept(serverSock, nullptr, nullptr);
        if (clientSock == -1) {
            cerr << "Failed to accept connection on " << node << endl;
            continue;
        }


        // Handle the client in a new thread
        thread([clientSock, ht]() {
            handleClient(clientSock, ht);
        }).detach();
    }

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
    thread serverThread(startHashTableServer, node, ref(this->hashRing), ref(this->nodes), ref(this->serverMap));  
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

    HashTable *ht = serverMap[node];
    ht->stopServerFunc(this->hashRing, this->nodes);
}

bool ConsistentHashRing::isNodeAlive(const string& nodeAddr) {
    size_t colonPos = nodeAddr.find(':');
    if (colonPos == string::npos) return {};

    string ip = nodeAddr.substr(0, colonPos);
    int port = stoi(nodeAddr.substr(colonPos + 1));

    // Connect and request keys
    int sock = RPC::connectToServer(ip, port);
    if (sock < 0) return false;

    string request = "HEARTBEAT\n";
    send(sock, request.c_str(), request.size(), 0);

    char buffer[4096] = {0};
    recv(sock, buffer, sizeof(buffer), 0);
    close(sock);

    string resp = string(buffer);
    if (resp == "ALIVE") return true;
    return false;
}

void* ConsistentHashRing::heartbeatMonitor(void* arg) {
    ConsistentHashRing *ring = static_cast<ConsistentHashRing*>(arg);
    while (true) {
        for (string value : ring->nodes) {
            cout << value << " ";
            if (!ring->isNodeAlive(value)) {
                cout << "[Heartbeat] Node down: " << value << " — removing from ring.\n";
            }
        }

        sleep(5);
    }

    return nullptr;
}
