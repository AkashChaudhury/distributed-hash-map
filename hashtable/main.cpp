/*
For test purpose
g++ -o hashtable main.cpp hash.cpp -pthread
./hashtable
*/

#include "hash.h"
#include <iostream>

int main() {
    HashTable ht(10);

    ht.insert("apple", 100);
    ht.insert("banana", 200);
    ht.insert("carrot", 300);
    ht.insert("apple", 150);  // update

    std::cout << "apple = " << ht.get("apple") << std::endl;
    std::cout << "banana = " << ht.get("banana") << std::endl;
    std::cout << "grape = " << ht.get("grape") << std::endl;

    ht.remove("apple");
    std::cout << "apple after remove = " << ht.get("apple") << std::endl;

    return 0;
}