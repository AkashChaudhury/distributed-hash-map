g++ main.cpp hash_ring_final.cpp -o hashring
./hashring


g++ -o consistent_hash_server server.cpp hash_ring_final.cpp -lpthread
./consistent_hash_server