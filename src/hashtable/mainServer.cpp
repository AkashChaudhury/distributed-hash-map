#include "hash.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <vector>
#include <set>
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

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./server <port> <nodename>\n";
        return 1;
    }

    int port = std::stoi(argv[1]);
    std::string myNodeName = argv[2];

    // Initialize node list (could be extended for multiple nodes)
    std::set<std::string> nodeList = { "127.0.0.1:5001", "127.0.0.1:5002" }; 
    HashTable* ht = new HashTable(50, myNodeName, 0, nodeList);

    // Create the server socket
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the server socket
    if (bind(serverSock, (sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "Binding failed with error: " << strerror(errno) << std::endl;
        close(serverSock);
        return 1;
    }

    // Start listening for connections
    if (listen(serverSock, 10) == -1) {
        std::cerr << "Listen failed." << std::endl;
        close(serverSock);
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        int clientSock = accept(serverSock, nullptr, nullptr);
        if (clientSock == -1) {
            std::cerr << "Error accepting client connection." << std::endl;
            continue;
        }

        // Use a separate thread to handle each client connection
        std::thread(handleClient, clientSock, ht).detach();
    }

    // Cleanup resources before exiting
    close(serverSock);
    delete ht;

    return 0;
}
