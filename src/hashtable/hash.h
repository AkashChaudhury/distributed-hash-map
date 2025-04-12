#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>
#include <pthread.h>
#include <unordered_map>
#include "../hash_functions/hash_functions.h"

#include <cstring>     // for memset
#include <unistd.h>    // for close()
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <set>


using namespace std;

class Node {
public:
    string key;
    // can be converted to any other data type
    void *value;
    Node *next;
    Node *prev;

    // want to make this head and tail node
    Node();

    // Constructor with value
    Node(string k);
    Node(string k, void *v);
};

class LinkedList {
    Node *head;
    Node *tail;
    int len;
public:
    LinkedList();
    void addNode(Node *n);
    void delNode(Node *n);
    Node* getNode(string key);
    inline Node* getHead() {
        return head;
    }
    inline Node* getTail() {
        return tail;
    }
    inline int getLen() {
        return len;
    }
};

class HashEntry : public LinkedList{
    int hashKey;
    void add(Node *val);
    void del(Node *val);

public:
    // Constructor to initialize key-value pair
    // Following LRU cache technique... 
    // Its easy :)
    HashEntry(int key);

    // check if key is present
    bool checkKey(string key);

    // always call this after checkKey
    // Return value of key
    void* returnVal(string key);

    // always call this after checkKey
    // delete key and value
    void delKey(string key);

    // always call this after checkKey
    // add key and value
    void addKey(string key, void *value);
};

class RPC {
public:
    // RPCs methods
    int connectToServer(const string& ip, int port);
    vector<string> split(const string& str, char delim);
    vector<string> getKeysFromNode(const string& nodeAddr);
    string getValueFromNode(const string& nodeAddr, const string& key);
    void deleteKeysOnNode(const string& nodeAddr, const vector<string>& keys);
    // RPC Server-handling methods
    
}; 

// ----------- HashTable Class -------------
class HashTable : private RPC {
private:
    vector<HashEntry*> buckets;
    int size;
    pthread_mutex_t lock;
    LinkedList metadata;
    unordered_map<string, Node*> metadataKey;
    string MyNodeName;

    // RPC Server-handling methods
    int connectToServer(const string& ip, int port);
    vector<string> split(const string& str, char delim);
    vector<string> getKeysFromNode(const string& nodeAddr);
    string getValueFromNode(const string& nodeAddr, const string& key);
    void deleteKeysOnNode(const string& nodeAddr, const vector<string>& keys);

    int hashFunc(const string& key);
public:
    HashTable(int sz, string name);
    HashTable(int sz, string name, int hash, set<string> Nodes);
    
    ~HashTable();
    void insert(const string& key, void *value);
    bool exists(const string& key);
    void* get(const string& key);
    void remove(const string& key);
    vector<string> exportAll();
    void removeKeys(const vector<string>& keys);

    // want to shift this to RPC class
    // but time limitations are there
    string handleRPCRequest(const string& request);
    string handleGetKeys();
    string handleGetValue(const string& key);
    string handleDeleteKeys(const string& keysStr);
    string handleSetValue(const string& key, const string& value);
};

#endif

