#include "hash.h"
#include <iostream>

Node::Node() {
    key = " ";
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

HashEntry::HashEntry(int key) {
    hashKey = key;
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->prev = head;
    len = 0;
}

void HashEntry::addNode(Node *n) {
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

void HashEntry::delNode(Node *n) {
    len--;
    Node *next = n->next;
    Node *prev = n->prev;
    next->prev = prev;
    prev->next = next;
    delete n;
}

void HashEntry::add(Node *val) {
    addNode(val);
}

void HashEntry::del(Node *val) {
    delNode(val);
}

bool HashEntry::checkKey(string key) {
    Node *temp = head->next;
    while (temp != tail) {
        if (temp->key == key) return true;
        temp = temp->next;
    }
    return false;
}

void* HashEntry::returnVal(string key) {
    Node *temp = head->next;
    while (temp != tail) {
        if (temp->key == key) return temp->value;
        temp = temp->next;
    }
    return 0;
}

void HashEntry::delKey(string key) {
    Node *temp = head->next;
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

// ---------------- HashTable ----------------

// each bucket has its own HashEntry
// cant use 
// buckets = vector<HashEntry*>(sz, new HashEntry(0));
// Learned something new :)
HashTable::HashTable(int sz) : size(sz) {
    buckets.reserve(sz);
    for (int i = 0; i < sz; ++i) {
        buckets.push_back(new HashEntry(i));
    }
    pthread_mutex_init(&lock, nullptr);
}

HashTable::~HashTable() {
    for (int i = 0; i < size; ++i)
        delete buckets[i];
    pthread_mutex_destroy(&lock);
}


// I personally had easier algo
// found this in google
// more optimized
int HashTable::hashFunc(const string& key) {
    long long hash = 5381;
    for (char c : key) {
        hash = hash * 33 + c;
    }
    return hash % size;
}

void HashTable::insert(const string& key, void *value) {
    pthread_mutex_lock(&lock);
    int idx = hashFunc(key);
    if (buckets[idx]->checkKey(key)) {
        buckets[idx]->delKey(key);
    }
    buckets[idx]->addKey(key, value);
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
    if (buckets[idx]->checkKey(key))
        buckets[idx]->delKey(key);
    pthread_mutex_unlock(&lock);
}
