
### **Stage 1:**

1. Only the Hash Map is properly distributed and thread safe

2. In this model, I gave more priority to High Availability than to Consistency.

3. For consistency, there should be at least **3 copies** of the data.

4. **During Adding and Removing Nodes:**

There are two strategies we can use to maintain consistency:

**a.** Let the **Consistent Hashing** logic take responsibility for transferring the data.  
**b.** Let the **nodes themselves** take responsibility for transferring the data.

**[Pros and Cons]**

Although this code is not a fully distributed system, when it comes to consistent hashing:

- If the `ConsistentHashRing` fails, and then comes back up, it can retrieve data from the existing hash tables.
- In **scenario (a)**, if the `ConsistentHashRing` fails, data might not get migrated.
- In **scenario (b)**, data will still be moved to another node. Even if a node fails, the data will remain on the original node, thus maintaining High Availability.

So I chose **Approach (a)**.

5. My Hash Table servers are always initiated by the `ConsistentHashRing` server.  
   They **cannot be independently created**.

6. Heartbeat monitoring of Hash Table servers is implemented — this will be useful in **Stage 2**.

7. **Used a hack:**

   a. The Hash Table can store any kind of data.  
   b. But the REST APIs currently support only strings (in JSON format).

---

### **How to Run:**

#### **Consistent Hashing Server**

This is for the JSON implementation:

```bash
git clone https://github.com/nlohmann/json.git

cd src/consistent_hashing/

g++ -o consistent_hash_server CC_Server.cpp hash_ring_final.cpp ../hashtable/hash.cpp ../hash_functions/hash_functions.cpp -lpthread

./consistent_hash_server
```

#### **Client:**

```bash
cd client

g++ -o client client.cpp

./client
```

---

### **Stage 2:**

- Create **replicas** of key/value pairs — **at least 3**.
- When a heartbeat is missed, create one more replica on another node.

**How to implement it:**

When we receive data, we can:

**Option 1:** Store keys like:
```
Key_#_1  
Key_#_2  
Key_#_3
```

Or use 3 different hash functions —  
**But** this doesn’t guarantee that data will be placed on **3 different nodes**.

**Option 2:** Store the key/value pair on **3 different nodes**, and maintain a **metadata DB** that tracks which servers hold which keys.

If a heartbeat is missed:

- Forcefully remove the node from the hash ring.
- Use the metadata DB to retrieve values from backup nodes.

Still brainstorming the best approach here.

---

### **Stage 3:**

- Adding and removing nodes is **supported**.
- Data will be **automatically transferred** to other nodes.
