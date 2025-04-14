### **This is a custom implementation of `unordered_map`**

As mentioned in the requirements, I could not use any third-party libraries.  
So I implemented this myself. I found the approach in a Udemy course and adapted it for this project.

---

### **Class Breakdown:**

#### **1. `Node` Class**
- Stores the **key/value** pair.
- Designed to be **scalable**, as the type of the value can be changed easily.

#### **2. `LinkedList` Class**
- A standard **Linked List** implementation.
- Used in multiple places across other classes (for chaining and metadata tracking).

#### **3. `HashEntry` Class**
- Maintains the mapping between a **hash key** and the corresponding `Node`.
- Handles **collisions** using a linked list of nodes with the same hash key.
- Essentially a modified version of a linked list for this purpose.

#### **4. `HashTable` Class**
- The **main map** implementation (analogous to `unordered_map`).
  
##### Elements used in this class:
- **`size`** – Tracks the current size of the Hash Table.
- **`lock`** – A basic `pthread_mutex_t` lock for thread safety.
- **`metadata`** – A linked list that stores all keys (a `vector` could also be used).
- **`metadataKey`** – Tracks where each key is located.
- **`stopServer`** – A flag used to stop the Hash Table server gracefully.

---

### **Implemented APIs:**

- **Get Key/Value**  
  Retrieve the value associated with a given key.

- **Get All Keys**  
  Return a list of all the keys stored in the hash table.

- **Delete Key/Value Pair**  
  Remove a key and its corresponding value from the hash table.

- **Set Key/Value Pair**  
  Insert or update a key/value pair in the hash table.

- **Check Key Existence**  
  Verify whether a given key exists in the hash table.

- **Heartbeat**  
  Check the health/status of the Hash Table server.
