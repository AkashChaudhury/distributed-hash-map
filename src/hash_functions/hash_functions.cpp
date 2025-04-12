
#include "hash_functions.h"

int HashFunc(const string& key, int size) {
    long long hash = 5381;
    for (char c : key) {
        hash = hash * 33 + c;
    }
    return hash % size;
}