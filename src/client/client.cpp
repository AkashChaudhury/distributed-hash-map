#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>  // for memcpy

using namespace std;

string sendRPC(const string& serverAddress, int port, const string& request) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return "Socket creation failed.";
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    struct hostent* server = gethostbyname(serverAddress.c_str());
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


void printMenu() {
    cout << "\n===== Distributed Hash Table Menu =====\n";
    cout << "1. Add Node\n";
    cout << "2. Get Node for Key\n";
    cout << "3. Remove Node\n";
    cout << "4. Get val\n";
    cout << "5. Set val\n";
    cout << "6. Remove Key\n";
    cout << "7. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    string serverAddress = "127.0.0.1";
    int port = 5000;

    while (true) {
        printMenu();

        int choice;
        cin >> choice;
        cin.ignore(); // Clear newline

        if (choice == 1) {
            string node;
            cout << "Enter node address (e.g., 127.0.0.1:5000): ";
            getline(cin, node);
            string request = "ADDNODE " + node;
            string response = sendRPC(serverAddress, port, request);
            cout << response << endl;
        }
        else if (choice == 2) {
            string key;
            cout << "Enter key: ";
            getline(cin, key);
            string request = "GETNODE " + key;
            string response = sendRPC(serverAddress, port, request);
            cout << response << endl;
        }
        else if (choice == 3) {
            string node;
            cout << "Enter node address to remove (e.g., 127.0.0.1:5000): ";
            getline(cin, node);
            string request = "REMOVENODE " + node;
            string response = sendRPC(serverAddress, port, request);
            cout << response << endl;
        }

        else if (choice == 4) {
            string key;
            cout << "Enter key to get value: ";
            getline(cin, key);
            string request = "GETNODE " + key;
            string response = sendRPC(serverAddress, port, request);
            cout << response << endl;

            if (response == "No node found") {
                cout << response;
            } else {
                string node = response;
                request = "GETVAL " + key;
                response = sendRPC(node, request);
                cout << "Response: " << response << endl;
            }
        }

        else if (choice == 5) {
            string key, value;
            cout << "Enter key: ";
            getline(cin, key);
            cout << "Enter value: ";
            getline(cin, value);

            string request = "GETNODE " + key;
            string response = sendRPC(serverAddress, port, request);
            cout << response << endl;

            if (response == "No node found") {
                cout << response;
            } else {
                string node = response;
                request = "SETVAL " + key + " " + value;
                response = sendRPC(node, request);
                cout << "Response: " << response << endl;
            }
        }

        else if (choice == 6) {
            string key;
            cout << "Enter key to get value: ";
            getline(cin, key);
            string request = "GETNODE " + key;
            string response = sendRPC(serverAddress, port, request);
            cout << response << endl;

            if (response == "No node found") {
                cout << response;
            } else {
                string node = response;
                request = "DELKEYS " + key;
                response = sendRPC(node, request);
                cout << "::DELETED";
            }
        }

        else if (choice == 7) {
            cout << "Exiting.\n";
            break;
        }
        else {
            cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}
