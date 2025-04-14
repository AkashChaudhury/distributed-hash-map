
Stage 1:
1. Only Hash Map is proderly Distributed.

2. In this model, I gave more priority to Highly Available the consystency.

3. For consystancy there should be altealst 3 copies of data.

4. During Adding and Removing Nodes:

There are 2 things we can do to maintain Consistancey:

a. Either let Consystancey hashing take responsibility of Transfering the data

b. Let the nodes take responsibility of Transfering the data

[Pros and Cons]

Althow this code is not a proper distributed system, when comes to consystancy hashing, if ConsistentHashRing fails, when it comes up, it can get data from hashtables.

If ConsistentHashRing fails, data may not get migrated in scenario 1
but in scenario 2, data will get to another node, even if node fails, the data will stay in older node, which will maitain HA.

So I went with approach 1

5. My Hash Table servers will initiate from ConsistentHashRing Server.
They can not be independently created

6. Heart Beat of Hash Table servers are also checked, will be usefull in Stage 2.

7. Used a hack 
    
    a. Hash Table can store any kind of data 

    b. But Rest API are only supporting String (Json)



How to Run:
Consistent hashing server

This is for json implementation

git clone https://github.com/nlohmann/json.git

cd src/consistant_hashing/

g++ -o consistent_hash_server CC_Server.cpp hash_ring_final.cpp ../hashtable/hash.cpp ../hash_functions/hash_functions.cpp -lpthread

./consistent_hash_server


Client:

cd client

g++ -o client client.cpp 

./client

Stage 2:
Create replicas of key/value pair
Atleast 3
When Heart beat is not passed, we can make one more replica

how to implement?
When we recive data, we can do something like
Key_#_1
Key_#_2
Key_#_3

But this does not give us guarenty that everything will go to Different Node

Doing more brainstorm shat to do

Stage 3: 
It is Supported, We can Add or delete Nodes.
And Data will transfer to other Nodes.



