#include <iostream>
#include <string>
#include <set>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include "hash_ring_final.h"  // Includes ConsistentHashRing and HashTable

void printMenu() {
    std::cout << "\n===== Distributed Hash Table Menu =====\n";
    std::cout << "1. Add Node\n";
    std::cout << "2. Get Node for Key\n";
    std::cout << "3. Get Value for Key\n";
    std::cout << "4. Set Key-Value\n";
    std::cout << "5. Exit\n";
    std::cout << "Choose an option: ";
}

std::string sendRPC(const std::string& node, const std::string& request) {
    auto pos = node.find(":");
    std::string hostname = node.substr(0, pos);
    int port = std::stoi(node.substr(pos + 1));

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return "Socket creation failed.";
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    struct hostent* server = gethostbyname(hostname.c_str());
    if (server == nullptr) {
        close(sock);
        return "Hostname resolution failed.";
    }
    memcpy(&serv_addr.sin_addr, server->h_addr, server->h_length);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sock);
        return "Connection failed.";
    }

    send(sock, request.c_str(), request.length(), 0);
    char buffer[4096] = {0};
    int valread = recv(sock, buffer, sizeof(buffer), 0);
    close(sock);

    return std::string(buffer, valread);
}

int main() {
    int virtualNodeCount = 3;
    ConsistentHashRing ring(virtualNodeCount);

    while (true) {
        printMenu();

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear newline

        if (choice == 1) {
            std::string node;
            std::cout << "Enter node address (e.g., 127.0.0.1:5000): ";
            std::getline(std::cin, node);
            ring.addNode(node);
            std::cout << "Node " << node << " added successfully.\n";
        }
        else if (choice == 2) {
            std::string key;
            std::cout << "Enter key: ";
            std::getline(std::cin, key);
            std::string node = ring.getNodeForKey(key);
            std::cout << "Key \"" << key << "\" maps to node: " << node << std::endl;
        }
        else if (choice == 3) {
            std::string key;
            std::cout << "Enter key to get value: ";
            std::getline(std::cin, key);
            std::string node = ring.getNodeForKey(key);

            if (node.empty()) {
                std::cout << "No nodes available in the ring.\n";
            } else {
                std::string request = "GETVAL " + key;
                std::string response = sendRPC(node, request);
                std::cout << "Response: " << response << std::endl;
            }
        }
        else if (choice == 4) {
            std::string key, value;
            std::cout << "Enter key: ";
            std::getline(std::cin, key);
            std::cout << "Enter value: ";
            std::getline(std::cin, value);
            std::string node = ring.getNodeForKey(key);

            if (node.empty()) {
                std::cout << "No nodes available in the ring.\n";
            } else {
                std::string request = "SETVAL " + key + " " + value;
                std::string response = sendRPC(node, request);
                std::cout << "Response: " << response << std::endl;
            }
        }
        else if (choice == 5) {
            std::cout << "Exiting.\n";
            break;
        }
        else {
            std::cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}
