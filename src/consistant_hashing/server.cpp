#include <iostream>
#include <string>
#include <set>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include "hash_ring_final.h" 
using namespace std;

void printMenu() {
    cout << "\n===== Distributed Hash Table Menu =====\n";
    cout << "1. Add Node\n";
    cout << "2. Get Node for Key\n";
    cout << "3. Get Value for Key\n";
    cout << "4. Set Key-Value\n";
    cout << "5. Remove Node\n"; 
    cout << "6. Exit\n";
    cout << "7. Delete\n";
    cout << "Choose an option: ";
}

string sendRPC(const string& node, const string& request) {
    auto pos = node.find(":");
    string hostname = node.substr(0, pos);
    int port = stoi(node.substr(pos + 1));

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

    return string(buffer, valread);
}

int main() {
    int virtualNodeCount = 3;
    ConsistentHashRing ring(virtualNodeCount);


    pthread_t hbThread;
    pthread_create(&hbThread, nullptr, ConsistentHashRing::heartbeatMonitor, &ring);
    pthread_detach(hbThread);

    while (true) {
        printMenu();

        int choice;
        cin >> choice;
        cin.ignore(); // Clear newline

        if (choice == 1) {
            string node;
            cout << "Enter node address (e.g., 127.0.0.1:5000): ";
            getline(cin, node);
            ring.addNode(node);
            cout << "Node " << node << " added successfully.\n";
        }
        else if (choice == 2) {
            string key;
            cout << "Enter key: ";
            getline(cin, key);
            string node = ring.getNodeForKey(key);
            cout << "Key \"" << key << "\" maps to node: " << node << endl;
        }
        else if (choice == 3) {
            string key;
            cout << "Enter key to get value: ";
            getline(cin, key);
            string node = ring.getNodeForKey(key);

            if (node.empty()) {
                cout << "No nodes available in the ring.\n";
            } else {
                string request = "GETVAL " + key;
                string response = sendRPC(node, request);
                cout << "Response: " << response << endl;
            }
        }
        else if (choice == 4) {
            string key, value;
            cout << "Enter key: ";
            getline(cin, key);
            cout << "Enter value: ";
            getline(cin, value);
            string node = ring.getNodeForKey(key);

            if (node.empty()) {
                cout << "No nodes available in the ring.\n";
            } else {
                string request = "SETVAL " + key + " " + value;
                string response = sendRPC(node, request);
                cout << "Response: " << response << endl;
            }
        }
        else if (choice == 5) {
            string node;
            cout << "Enter node address to remove (e.g., 127.0.0.1:5000): ";
            getline(cin, node);
            ring.removeNode(node);
            cout << "Node " << node << " removed successfully.\n";
        }
        else if (choice == 7) {
            string key;
            cout << "Enter key to delete value: ";
            getline(cin, key);
            string node = ring.getNodeForKey(key);

            if (node.empty()) {
                cout << "No nodes available in the ring.\n";
            } else {
                string request = "DELKEYS " + key;
                string response = sendRPC(node, request);
                cout << "::DELETED";
            }
        }
        else if (choice == 6) {
            cout << "Exiting.\n";
            break;
        }
        else {
            cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}
