#include "hash_ring_final.h"
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>

#define PORT 8080

using namespace std;

ConsistentHashRing ring(3); // 3 virtual nodes per real node

void handle_client(int client_socket) {
    char buffer[1024] = {0};
    int valread = read(client_socket, buffer, 1024);
    if (valread <= 0) {
        close(client_socket);
        return;
    }

    string request(buffer);
    if (request.find("GETNODE") == 0) {
        string key = request.substr(8); // after "GETNODE "
        string node = ring.getNodeForKey(key);
        if (node.empty()) {
            string msg = "ERROR: No nodes available\n";
            send(client_socket, msg.c_str(), msg.size(), 0);
        } else {
            send(client_socket, node.c_str(), node.size(), 0);
        }
    } else {
        string msg = "ERROR: Unknown command\n";
        send(client_socket, msg.c_str(), msg.size(), 0);
    }

    close(client_socket);
}

int main() {
    // Add real hash table servers here
    ring.addNode("127.0.0.1:9090");
    ring.addNode("127.0.0.1:9091");
    ring.addNode("127.0.0.1:9092");

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

    cout << "Consistent Hash Server running on port " << PORT << endl;

    while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        thread(handle_client, client_socket).detach();
    }

    close(server_fd);
    return 0;
}

