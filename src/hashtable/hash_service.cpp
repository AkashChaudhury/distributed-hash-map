#include "hash.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>

using namespace std;

HashTable table(100);  // hash table with 100 buckets

void handle_client(int client_socket) {
    char buffer[1024] = {0};
    int valread = read(client_socket, buffer, 1024);
    if (valread <= 0) {
        close(client_socket);
        return;
    }

    stringstream ss(buffer);
    string cmd, key, val;
    ss >> cmd >> key;
    if (cmd == "SET") {
        ss >> val;
        char* val_copy = new char[val.size() + 1];
        strcpy(val_copy, val.c_str());
        table.insert(key, val_copy);
        string msg = "STORED\n";
        send(client_socket, msg.c_str(), msg.size(), 0);
    } else if (cmd == "GET") {
        if (table.exists(key)) {
            char* val = (char*)table.get(key);
            string msg = string(val) + "\n";
            send(client_socket, msg.c_str(), msg.size(), 0);
        } else {
            string msg = "NOT_FOUND\n";
            send(client_socket, msg.c_str(), msg.size(), 0);
        }
    } else if (cmd == "DEL") {
        if (table.exists(key)) {
            table.remove(key);
            string msg = "DELETED\n";
            send(client_socket, msg.c_str(), msg.size(), 0);
        } else {
            string msg = "NOT_FOUND\n";
            send(client_socket, msg.c_str(), msg.size(), 0);
        }
    } else {
        string msg = "ERROR: Unknown command\n";
        send(client_socket, msg.c_str(), msg.size(), 0);
    }

    close(client_socket);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <port>" << endl;
        return 1;
    }

    int PORT = stoi(argv[1]);

    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    cout << "Hash Table Server running on port " << PORT << endl;

    while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        thread(handle_client, client_socket).detach();
    }

    close(server_fd);
    return 0;
}
