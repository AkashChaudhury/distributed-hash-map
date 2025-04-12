g++ main.cpp hash_ring_final.cpp -o hashring
./hashring


ubuntu@myDev:~/Desktop/distributed-hash-map/src/consistant_hashing$ g++ -o consistent_hash_server server.cpp hash_ring_final.cpp ../hashtable/hash.cpp ../hash_functions/hash_functions.cpp -lpthread
ubuntu@myDev:~/Desktop/distributed-hash-map/src/consistant_hashing$ ./consistent_hash_server 
Enter a node address (localhost:<port>) to add or type 'done' to finish: localhost:5000
Node localhost:5000 added successfully.
Enter a node address (localhost:<port>) to add or type 'done' to finish: 
Get Keys from localhost:5000
Hash table server listening on localhost:5000
localhost:5001
Node localhost:5001 added successfully.
Enter a node address (localhost:<port>) to add or type 'done' to finish: 
Get Keys from localhost:5000

Check Keys from localhost:5000 key = key0 should be in localhost:5000

Check Keys from localhost:5000 key = key1 should be in localhost:5000

Check Keys from localhost:5000 key = key2 should be in localhost:5000

Check Keys from localhost:5000 key = key3 should be in localhost:5000

Check Keys from localhost:5000 key = key4 should be in localhost:5000

Check Keys from localhost:5000 key = key5 should be in localhost:5000

Check Keys from localhost:5000 key = key6 should be in localhost:5000

Check Keys from localhost:5000 key = key7 should be in localhost:5000

Check Keys from localhost:5000 key = key8 should be in localhost:5000

Check Keys from localhost:5000 key = key9 should be in localhost:5000

Get Keys from localhost:5001
Hash table server listening on localhost:5001
localhost:5002
Node localhost:5002 added successfully.
Enter a node address (localhost:<port>) to add or type 'done' to finish: 
Get Keys from localhost:5000

Check Keys from localhost:5000 key = key0 should be in localhost:5000

Check Keys from localhost:5000 key = key1 should be in localhost:5000

Check Keys from localhost:5000 key = key2 should be in localhost:5000

Check Keys from localhost:5000 key = key3 should be in localhost:5000

Check Keys from localhost:5000 key = key4 should be in localhost:5000

Check Keys from localhost:5000 key = key5 should be in localhost:5000

Check Keys from localhost:5000 key = key6 should be in localhost:5000

Check Keys from localhost:5000 key = key7 should be in localhost:5000

Check Keys from localhost:5000 key = key8 should be in localhost:5000

Check Keys from localhost:5000 key = key9 should be in localhost:5000

Get Keys from localhost:5001

Get Keys from localhost:5002
Hash table server listening on localhost:5002
localhost:5003
Node localhost:5003 added successfully.
Enter a node address (localhost:<port>) to add or type 'done' to finish: 
Get Keys from localhost:5000

Check Keys from localhost:5000 key = key0 should be in localhost:5003

Insert Key from node localhost:5000 key = key0

Check Keys from localhost:5000 key = key1 should be in localhost:5003

Insert Key from node localhost:5000 key = key1

Check Keys from localhost:5000 key = key2 should be in localhost:5003

Insert Key from node localhost:5000 key = key2

Check Keys from localhost:5000 key = key3 should be in localhost:5003

Insert Key from node localhost:5000 key = key3

Check Keys from localhost:5000 key = key4 should be in localhost:5003

Insert Key from node localhost:5000 key = key4

Check Keys from localhost:5000 key = key5 should be in localhost:5003

Insert Key from node localhost:5000 key = key5

Check Keys from localhost:5000 key = key6 should be in localhost:5003

Insert Key from node localhost:5000 key = key6

Check Keys from localhost:5000 key = key7 should be in localhost:5003

Insert Key from node localhost:5000 key = key7

Check Keys from localhost:5000 key = key8 should be in localhost:5003

Insert Key from node localhost:5000 key = key8

Check Keys from localhost:5000 key = key9 should be in localhost:5003

Insert Key from node localhost:5000 key = key9

Get Keys from localhost:5001

Get Keys from localhost:5002

Get Keys from localhost:5003
Hash table server listening on localhost:5003
^C
ubuntu@myDev:~/Desktop/distributed-hash-map/src/consistant_hashing$ 
ubuntu@myDev:~/Desktop/distributed-hash-map/src/consistant_hashing$ 
