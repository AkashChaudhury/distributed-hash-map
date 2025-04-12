#include "hash.h"
#include <iostream>
#include <cstdlib>  // for malloc and free

void printValue(void *value) {
    if (value) {
        int *int_ptr = (int *)value;  // Cast void* to int*
        printf("Value: %d\n", *int_ptr);
    } else {
        printf("Value: NULL\n");
    }
}

int main() {
    HashTable ht(10, "Node1");

    int *val1 = new int(100);
    int *val2 = new int(200);
    int *val3 = new int(300);
    int *val4 = new int(150);  // updated value for apple

    ht.insert("apple", val1);
    ht.insert("banana", val2);
    ht.insert("carrot", val3);
    ht.insert("apple", val4);  // update

    printValue(ht.get("apple"));
    printValue(ht.get("banana"));
    printValue(ht.get("grape"));  // not present

    ht.remove("apple");
    printValue(ht.get("apple"));  // should be null

    // Clean up allocated memory (you can also wrap this logic in HashTable later)
    delete val1;
    delete val2;
    delete val3;
    delete val4;

    return 0;
}
