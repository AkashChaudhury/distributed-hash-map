#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>
#include <pthread.h>

using namespace std;

class Node {
public:
    string key;
    // can be converted to any other data type
    int value;
    Node *next;
    Node *prev;

    // want to make this head and tail node
    Node();

    // Constructor with value
    Node(string k, int v);
};

class HashEntry {
    int hashKey;
    Node *head;
    Node *tail;
    int len;

    void addNode(Node *n);
    void delNode(Node *n);
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
    int returnVal(string key);

    // always call this after checkKey
    // delete key and value
    void delKey(string key);

    // always call this after checkKey
    // add key and value
    void addKey(string key, int value);
};

// ----------- HashTable Class -------------
class HashTable {
private:
    vector<HashEntry*> buckets;
    int size;
    pthread_mutex_t lock;

    int hashFunc(const string& key);

public:
    HashTable(int sz);
    ~HashTable();
    void insert(const string& key, int value);
    bool exists(const string& key);
    int get(const string& key);
    void remove(const string& key);
};

#endif // HASH_H


// I personally had easier algo
// found this in google
// more optimized

