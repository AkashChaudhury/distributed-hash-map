#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

string getHashNode(const string& key) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080); // Consistent Hash Server port
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    string req = "GETNODE " + key;
    send(sock, req.c_str(), req.size(), 0);

    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    close(sock);

    return string(buffer);
}

string talkToNode(string ip, int port, string command) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    send(sock, command.c_str(), command.size(), 0);

    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    close(sock);

    return string(buffer);
}

int main() {
    string line;
    while (true) {
        cout << "Enter command (SET key val | GET key | DEL key | EXIT): ";
        getline(cin, line);
        if (line == "EXIT") break;

        stringstream ss(line);
        string cmd, key;
        ss >> cmd >> key;

        if (cmd != "SET" && cmd != "GET" && cmd != "DEL") {
            cout << "Invalid command\n";
            continue;
        }

        string node = getHashNode(key);
        if (node.find(':') == string::npos) {
            cout << "Error from hash ring: " << node << endl;
            continue;
        }

        string ip = node.substr(0, node.find(':'));
        int port = stoi(node.substr(node.find(':') + 1));

        string response = talkToNode(ip, port, line);
        cout << "[Handled by " << node << "] " << response;
    }

    return 0;
}
