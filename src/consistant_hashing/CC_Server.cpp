#include <iostream>
#include <string>
#include <set>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include "hash_ring_final.h" 
using namespace std;

int virtualNodeCount = 3;
ConsistentHashRing ring(virtualNodeCount);

void* handleClient(void* arg) {
    int clientSock = *((int*)arg);
    char buffer[4096] = {0};
    int valread = recv(clientSock, buffer, sizeof(buffer), 0);

    if (valread > 0) {
        string request(buffer, valread);
        string response;

        // Handle the different commands
        if (request.find("ADDNODE") == 0) {
            string node = request.substr(8); // Extract node address
            ring.addNode(node);
            response = "Node " + node + " added successfully.";
        }
        else if (request.find("GETNODE") == 0) {
            string key = request.substr(8); // Extract key
            string node = ring.getNodeForKey(key);
            if (node.empty()) {
                response = "No node found";
            } else {
                response = node;
            }
        }
        else if (request.find("REMOVENODE") == 0) {
            string node = request.substr(11); // Extract node address
            ring.removeNode(node);
            response = "Node " + node + " removed successfully.";
        }
        else {
            response = "Invalid command.";
        }

        // Send the response back to the client
        send(clientSock, response.c_str(), response.length(), 0);
    }

    close(clientSock);
    return nullptr;
}

// Server setup for consistent hashing and managing hash tables
void startServer(const string& address, int port, ConsistentHashRing& ring) {
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        cerr << "Socket creation failed." << endl;
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Binding failed." << endl;
        close(serverSock);
        return;
    }

    if (listen(serverSock, 5) < 0) {
        cerr << "Listening failed." << endl;
        close(serverSock);
        return;
    }

    cout << "Server listening on " << address << ":" << port << endl;

    while (true) {
        int clientSock = accept(serverSock, nullptr, nullptr);
        if (clientSock >= 0) {
            pthread_t clientThread;
            pthread_create(&clientThread, nullptr, handleClient, &clientSock);
            pthread_detach(clientThread);
        }
    }

    close(serverSock);
}

int main() {
    // Server address and port
    string serverAddress = "127.0.0.1";
    int port = 5000;

    // Start the heartbeat monitor thread
    pthread_t heartbeatThread;
    pthread_create(&heartbeatThread, nullptr, ConsistentHashRing::heartbeatMonitor, &ring);
    pthread_detach(heartbeatThread);

    
    // Start the server for consistent hashing
    startServer(serverAddress, port, ring);

    return 0;
}
