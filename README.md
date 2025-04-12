# distributed-hash-map

Problem statement:

Stage 1: (Checked)

Introduction
  A common data structure used in any software system is a ‘hash  table’ (and there are several varieties of it).  For an in-memory hash table, memory foot print is extremely critical. As data continues to grow on a massive  scale (millions and billions of KV pairs), resource constraints begin to surface and it's not possible to store KV pairs for an in-mem HT, without being swapped. One solution is to have a distributed hash table where each node stores some batch of KV pairs (part of the complete namespace). 

Problem statement: 
  1) Implement a distributed hash table where the key processing daemons (aka HT Serving instances) are distinct processes (two or more).

     First you could implement them running on the same node, later they can be deployed on multiple nodes.

  2) Identify distributed hashing strategy. The reason we are implementing distributed hash table is because of limited system memory on individual nodes. We would like to keep the hash entries in memory as much as possible (low swapping).

  3) Based on key distribution strategy client should be able to identify a particular server for querying or inserting hash entry. Design can also have a co-ordinator node (optional). The sharding strategy could be based on consistent hashing or but not limited to it. Can use any advanced algorithm

  4) Use your choice of transport layer.

  5) key type: string , value type: could be of any type. The client should be able to understand how to read/process the value after fetching the entry. (For eg. json could be one such format OR you could keep in binary format too)

  6) Design and Implement client side APIs for CRUD operations. Read operation could be exposed as an iterator. Support for bulk operations is optional.

  7) Each HT instance  must be thread safe, ie multiple clients can connect to the same server to insert/delete one or more entries.

  8) Each server may implement thread pool (to be able to scale)
  
  9) It is important to ensure that there are no skews while key to node allocations are made.


Notes
  - Prepare a design draft. 
  - For simplicity and testing purpose run the multiple instances of the HT serving instaces on the same node, but they should be 
designed to be deployable across nodes. 
  - Do not use 3rd party  libraries (build the hash table from basic). 
  - You can be as innovative as possible in deciding the individual hash table design (and in-mem structure/form factor)
    or choice of the hash function.

Stage 2: (Brain Storming)

Stage 3: (Checked)
