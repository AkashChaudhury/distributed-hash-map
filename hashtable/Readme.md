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
