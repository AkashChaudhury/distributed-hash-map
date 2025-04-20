#include "hash.h"
#include <iostream>

Node::Node() {
    key = " ";
    value = nullptr;
    next = nullptr;
    prev = nullptr;
}

Node::Node(string k) {
    key = k;
    value = nullptr;
    next = nullptr;
    prev = nullptr;
}

Node::Node(string k, void *v) {
    key = k;
    value = v;
    next = nullptr;
    prev = nullptr;
}

LinkedList::LinkedList() {
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->prev = head;
    len = 0;
}

void LinkedList::addNode(Node *n) {
    len++;
    if (head->next == tail) {
        head->next = n;
        n->prev = head;
        tail->prev = n;
        n->next = tail;
    } else {
        Node *temp = tail->prev;
        temp->next = n;
        n->prev = temp;
        tail->prev = n;
        n->next = tail;
    }
}

void LinkedList::delNode(Node *n) {
    len--;
    Node *next = n->next;
    Node *prev = n->prev;
    next->prev = prev;
    prev->next = next;
    delete n;
}

Node* LinkedList::getNode(string key) {
    Node *temp = head->next;
    while (temp != tail) {
        if (temp->key == key) return temp;
        temp = temp->next;
    }
    return nullptr;
}

HashEntry::HashEntry(int key):LinkedList() {
    hashKey = key;
}

void HashEntry::add(Node *val) {
    addNode(val);
}

void HashEntry::del(Node *val) {
    delNode(val);
}

bool HashEntry::checkKey(string key) {
    Node *temp = getNode(key);
    return temp?true:false;
}

void* HashEntry::returnVal(string key) {
    return getNode(key)->value;
}

void HashEntry::delKey(string key) {
    Node *temp = getHead()->next;
    Node *tail = getTail();
    while (temp != tail) {
        if (temp->key == key) {
            del(temp);
            return;
        }
        temp = temp->next;
    }
}

void HashEntry::addKey(string key, void *value) {
    Node *temp = new Node(key, value);
    add(temp);
}


vector<string> RPC::split(const string& str, char delim) {
    vector<string> out;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim)) {
        out.push_back(token);
    }
    return out;
}

int RPC::connectToServer(const string& ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(sock);
        return -1;
    }

    return sock;
}

vector<string> RPC::getKeysFromNode(const string& nodeAddr) {
    // Parse IP:port
    size_t colonPos = nodeAddr.find(':');
    if (colonPos == string::npos) return {};

    string ip = nodeAddr.substr(0, colonPos);
    int port = stoi(nodeAddr.substr(colonPos + 1));

    // Connect and request keys
    int sock = RPC::connectToServer(ip, port);
    if (sock < 0) return {};

    string request = "GETKEYS\n";
    send(sock, request.c_str(), request.size(), 0);

    char buffer[4096] = {0};
    recv(sock, buffer, sizeof(buffer), 0);
    close(sock);

    return split(string(buffer), ',');
}


string HashTable::getValueFromNode(const string& nodeAddr, const string& key) {
    // Parse IP:port
    size_t colonPos = nodeAddr.find(':');
    if (colonPos == string::npos) return "";

    string ip = nodeAddr.substr(0, colonPos);
    int port = stoi(nodeAddr.substr(colonPos + 1));

    // Connect to the server
    int sock = RPC::connectToServer(ip, port);
    if (sock < 0) return "";

    string request = "GETVAL " + key + "\n";
    send(sock, request.c_str(), request.size(), 0);

    char buffer[4096] = {0};
    int bytesRead = recv(sock, buffer, sizeof(buffer), 0);
    close(sock);

    if (bytesRead <= 0) return "";

    return string(buffer);
}

void RPC::setValueToNode(const string& nodeAddr, const string& key, const string& val) {
    // Parse IP:port
    size_t colonPos = nodeAddr.find(':');
    if (colonPos == string::npos) return;

    string ip = nodeAddr.substr(0, colonPos);
    int port = stoi(nodeAddr.substr(colonPos + 1));

    // Connect to the server
    int sock = connectToServer(ip, port);
    if (sock < 0) return;

    string request = "SETVAL " + key + " " + val;
    send(sock, request.c_str(), request.size(), 0);

    char buffer[4096] = {0};
    int bytesRead = recv(sock, buffer, sizeof(buffer), 0);
    close(sock);

    if (bytesRead <= 0) return;

    return;
}


void HashTable::deleteKeysOnNode(const string& nodeAddr, const vector<string>& keys) {
    // Parse IP:port
    size_t colonPos = nodeAddr.find(':');
    if (colonPos == string::npos) return;

    string ip = nodeAddr.substr(0, colonPos);
    int port = stoi(nodeAddr.substr(colonPos + 1));

    // Connect to the server
    int sock = RPC::connectToServer(ip, port);
    if (sock < 0) return;

    ostringstream oss;
    oss << "DELKEYS ";
    for (size_t i = 0; i < keys.size(); ++i) {
        oss << keys[i];
        if (i != keys.size() - 1) oss << ",";
    }
    oss << "\n";

    string request = oss.str();
    send(sock, request.c_str(), request.size(), 0);

    close(sock);
}

string RPC::heartBeatResp() {
    return "ALIVE";
}

// ---------------- HashTable ----------------

// each bucket has its own HashEntry
// cant use 
// buckets = vector<HashEntry*>(sz, new HashEntry(0));
// Learned something new :)
HashTable::HashTable(int sz, string name) : size(sz) {
    stopServer = false;
    MyNodeName = name;
    buckets.reserve(sz);
    for (int i = 0; i < sz; ++i) {
        buckets.push_back(new HashEntry(i));
    }
    pthread_mutex_init(&lock, nullptr);
}

// Get Node Address
string check(string key, map<size_t, string> hashRing) {
    int hashVal = HashFunc(key, MAX_HASH);
    auto it = hashRing.lower_bound(hashVal);
    if (it == hashRing.end()) return hashRing.begin()->second;
    return it->second;
}

HashTable::HashTable(int sz, string name, map<size_t, string> hash, set<string> Nodes) : size(sz) {
    stopServer = false;
    MyNodeName = name;
    buckets.reserve(sz);
    for (int i = 0; i < sz; ++i) {
        buckets.push_back(new HashEntry(i));
    }
    pthread_mutex_init(&lock, nullptr);
    int myHash = HashFunc(name, size);
    for (const string& nodeAddr : Nodes) {
        // Skip fetching from self
        if (HashFunc(nodeAddr, MAX_HASH) == myHash) continue;

        // 1. Ask other node to export all its keys
        vector<string> remoteKeys = getKeysFromNode(nodeAddr);

        vector<string> toTake;
        for (const string& key : remoteKeys) {
            if (check(key, hash) == name) {
                // 2. Get the value from the remote node
                string val = getValueFromNode(nodeAddr, key); 

                // 3. Insert into local table
                string* valPtr = new string(val);
                insert(key, (void*)valPtr);   
                // 4. Schedule for deletion on remote
                toTake.push_back(key); 
            }
        }

        // 5. Ask the remote node to delete the moved keys
        if (!toTake.empty()) {
            deleteKeysOnNode(nodeAddr, toTake);
        }
    }
}

void HashTable::stopServerFunc(map<size_t, string> hash, set<string> Nodes) {
    vector<string> localKeys = exportAll();
    for (const string& key : localKeys) {
        string nodeAddr = check(key, hash);

        void* val = get(key);
        string* valPtr = static_cast<string*>(val);
        RPC::setValueToNode(nodeAddr, key, *valPtr);

        remove(key);
    }
    stopServer.store(true);
}

HashTable::~HashTable() {
    stopServer.store(true);
    for (int i = 0; i < size; ++i)
        delete buckets[i];
    pthread_mutex_destroy(&lock);
}


// I personally had easier algo
// found this in google
// more optimized
int HashTable::hashFunc(const string& key) {
    return HashFunc(key, size);
}

void HashTable::insert(const string& key, void *value) {
    pthread_mutex_lock(&lock);
    int idx = hashFunc(key);
    if (buckets[idx]->checkKey(key)) {
        buckets[idx]->delKey(key);
        metadataKey.erase(key);
    }
    buckets[idx]->addKey(key, value);

    Node *temp = new Node(key);
    metadata.addNode(temp);
    metadataKey[key] = temp;

    pthread_mutex_unlock(&lock);
}

bool HashTable::exists(const string& key) {
    pthread_mutex_lock(&lock);
    int idx = hashFunc(key);
    bool found = buckets[idx]->checkKey(key);
    pthread_mutex_unlock(&lock);
    return found;
}

void* HashTable::get(const string& key) {
    pthread_mutex_lock(&lock);
    int idx = hashFunc(key);
    void *val = buckets[idx]->checkKey(key) ? buckets[idx]->returnVal(key) : nullptr;
    pthread_mutex_unlock(&lock);
    return val;
}

void HashTable::remove(const string& key) {
    pthread_mutex_lock(&lock);
    int idx = hashFunc(key);
    if (buckets[idx]->checkKey(key)) {
        buckets[idx]->delKey(key);
        Node *temp = metadata.getNode(key);
        metadata.delNode(temp);
        metadataKey.erase(key);
    }
        
    pthread_mutex_unlock(&lock);
}

vector<string> HashTable::exportAll() {
    pthread_mutex_lock(&lock);
    vector<string> result;

    for (int i = 0; i < size; ++i) {
        Node *curr = buckets[i]->getHead()->next;
        Node *tail = buckets[i]->getTail();

        while (curr != tail) {
            result.push_back(curr->key);
            curr = curr->next;
        }
    }
    pthread_mutex_unlock(&lock);
    return result;
}

void HashTable::removeKeys(const vector<string>& keys) {
    for (const string& key : keys) {
        // Remove already have lock 
        // No more lock
        remove(key);
    }
}



// Handle "GETKEYS"
string HashTable::handleGetKeys() {
    vector<string> keys = exportAll();
    ostringstream oss;
    for (size_t i = 0; i < keys.size(); ++i) {
        oss << keys[i];
        if (i != keys.size() - 1) oss << ",";
    }
    return oss.str();
}

// Retrieve string value from the hash table
string HashTable::handleGetValue(const string& key) {
    void* val = get(key);
    if (val == nullptr) {
        return "NOT_FOUND";
    }
    string* valPtr = static_cast<string*>(val);
    return *valPtr; 
}

// Handle "DELKEYS key1,key2,..."
string HashTable::handleDeleteKeys(const string& keysStr) {
    vector<string> keys;
    stringstream ss(keysStr);
    string key;
    while (getline(ss, key, ',')) {
        keys.push_back(key);
    }
    removeKeys(keys);
    return "OK";
}

// Handle "SETVAL key1,key2,..."
string HashTable::handleSetValue(const string& key, const string& value) {
    string* valuePtr = new string(value);
    if (!isValidJson(value)) {
        return "Json Invalid";
    }
    insert(key, static_cast<void*>(valuePtr));
    return "OK";
}

// Dispatcher: handle full request string
string HashTable::handleRPCRequest(const string& input) {
    istringstream iss(input);
    string cmd;
    iss >> cmd;

    if (cmd == "GETKEYS") {
        return handleGetKeys();
    } else if (cmd == "GETVAL") {
        string key;
        iss >> key;
        return handleGetValue(key);
    } else if (cmd == "DELKEYS") {
        string rest;
        getline(iss, rest);
        if (!rest.empty() && rest[0] == ' ') rest = rest.substr(1);
        return handleDeleteKeys(rest);
    } else if (cmd == "SETVAL") {
        string key, value;
        iss >> key >> value;
        return handleSetValue(key, value);
    } else if (cmd == "HEARTBEAT") {
        return heartBeatResp();
    }
    else {
        return "ERR_UNKNOWN_COMMAND";
    }
}

vector<string> HashTable::split(const string& str, char delim) {
    return RPC::split(str, delim);
}

int HashTable::connectToServer(const string& ip, int port) {
    return RPC::connectToServer(ip, port);
}

vector<string> HashTable::getKeysFromNode(const string& nodeAddr) {
    return RPC::getKeysFromNode(nodeAddr);
}

string HashTable::heartBeatResp() {
    return RPC::heartBeatResp();
}

bool HashTable::isValidJson(const std::string& input) {
    try {
        auto j = json::parse(input);
        return true; // Successfully parsed
    } catch (json::parse_error& e) {
        return false; // Failed to parse
    }
}