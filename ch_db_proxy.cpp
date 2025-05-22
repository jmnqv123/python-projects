#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <thread>
#include <mutex>

using namespace std;

// Simple hash function wrapper for consistent hashing
size_t hash_fn(const string& key) {
    return std::hash<string>{}(key);
}

class ConsistentHash {
    map<size_t, string> hash_ring;
    vector<string> nodes;
    int virtual_nodes;

public:
    ConsistentHash(int vnodes = 3) : virtual_nodes(vnodes) {}

    void add_node(const string& node) {
        nodes.push_back(node);
        for (int i = 0; i < virtual_nodes; ++i) {
            string vnode = node + "#VN" + to_string(i);
            hash_ring[hash_fn(vnode)] = node;
        }
    }

    string get_node(const string& key) {
        if (hash_ring.empty()) return "";
        size_t key_hash = hash_fn(key);
        auto it = hash_ring.lower_bound(key_hash);
        if (it == hash_ring.end()) return hash_ring.begin()->second;
        return it->second;
    }

    void print_ring() {
        cout << "Hash Ring:\n";
        for (auto& [h, node] : hash_ring) {
            cout << "  " << setw(12) << h << " -> " << node << "\n";
        }
    }
};

int main() {
    ConsistentHash ch(3);  // 3 virtual nodes per real node
    ch.add_node("DB-Node-A");
    ch.add_node("DB-Node-B");
    ch.add_node("DB-Node-C");

    ch.print_ring();

    cout << "\nRouting keys to DB nodes:\n";
    vector<string> keys = {"customer_123", "order_456", "invoice_789", "user_001", "product_777"};

    for (const auto& key : keys) {
        string node = ch.get_node(key);
        cout << "Key '" << key << "' -> " << node << "\n";
    }

    return 0;
}
