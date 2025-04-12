This is copy of unordered_map
As mentioned in question I cannot use other library, I created this myself
Found this method in a Udemy course.

Node class 
1. contains the key/value pair of an 
2. Scalable as type of value can be changed

HashEntry Class
1. Keep track of hashkey Vs Node class
2. Keys having same hashkeys are in form of Linked list
3. This is more of a Linked List class with mods

HashTable Class
1. Main Map implementation

For testing we can run this
g++ -o hashtable main.cpp hash.cpp -pthread
./hashtable



ubuntu@myDev:~/Desktop/distributed-hash-map/src/hashtable$ g++ -o hashtable mainServer.cpp hash.cpp ../hash_functions/hash_functions.cpp  -pthread
ubuntu@myDev:~/Desktop/distributed-hash-map/src/hashtable$ ./hashtable 5001 node1
Server listening on port 5001

ubuntu@myDev:~/Desktop/distributed-hash-map/src/hashtable$ echo "SETVAL key2 value2" | nc 127.0.0.1 5001
OKubuntu@myDev:~/Desktop/distributed-hash-map/src/hashtable$ echo "GETKEYS" | nc 127.0.0.1 5001
key2ubuntu@myDev:~/Desktop/distributed-hash-map/src/hashtable$ echo "GETVAL key2" | nc 127.0.0.1 5001
value2ubuntu@myDev:~/Desktop/distributed-hash-map/src/hashtable$ echo "GETVAL key2" | nc 127.0.0.1 5002^C
ubuntu@myDev:~/Desktop/distributed-hash-map/src/hashtable$