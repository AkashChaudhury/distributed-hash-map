This is copy of unordered_map
As mentioned in question I cannot use other library, I created this myself
Found this method in a Udemy course.

Node class 
1. contains the key/value pair of an 
2. Scalable as type of value can be changed

LinkedList Class
1. Normal LinkedList class
2. Will be used multiple way in below class

HashEntry Class
1. Keep track of hashkey Vs Node class
2. Keys having same hashkeys are in form of Linked list
3. This is more of a Linked List class with mods

HashTable Class
1. Main Map implementation
Elements used in this class:
size: to get size of hashTable,
lock: Basic mutex Lock
metadata: stores all keys in LL, we can use vector too
metadataKey: to get which key is where
stopServer: to stop HashTable server

API:
Get Keys and valued
Get All keys
Delete Key/Value pair
Set Key/Value pair
Check if a key value pair exists or not
