#include <iostream>
#include <array>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>

constexpr int DEGREE = 3; // B+ Tree order

// -------------------- B+ Tree Node --------------------
struct BPlusNode {
    bool isLeaf = true;
    int size = 0;
    std::array<int, DEGREE> keys{};
    std::array<BPlusNode*, DEGREE + 1> children{};

    BPlusNode() {
        children.fill(nullptr);
    }
};

// -------------------- B+ Tree --------------------
class BPlusTree {
public:
    BPlusTree() {
        root = new BPlusNode();
    }

    void insert(int key) {
        BPlusNode* r = root;
        if (r->size == DEGREE) {
            BPlusNode* newRoot = new BPlusNode();
            newRoot->isLeaf = false;
            newRoot->children[0] = root;
            splitChild(newRoot, 0);
            root = newRoot;
        }
        insertNonFull(root, key);
    }

    bool search(int key) {
        return searchRecursive(root, key);
    }

private:
    BPlusNode* root;

    void splitChild(BPlusNode* parent, int idx) {
        BPlusNode* fullNode = parent->children[idx];
        BPlusNode* newNode = new BPlusNode();
        newNode->isLeaf = fullNode->isLeaf;
        newNode->size = DEGREE / 2;

        for (int i = 0; i < DEGREE / 2; ++i)
            newNode->keys[i] = fullNode->keys[i + DEGREE / 2];

        if (!fullNode->isLeaf) {
            for (int i = 0; i <= DEGREE / 2; ++i)
                newNode->children[i] = fullNode->children[i + DEGREE / 2];
        }

        fullNode->size = DEGREE / 2;

        for (int i = parent->size; i > idx; --i)
            parent->children[i + 1] = parent->children[i];

        parent->children[idx + 1] = newNode;

        for (int i = parent->size - 1; i >= idx; --i)
            parent->keys[i + 1] = parent->keys[i];

        parent->keys[idx] = fullNode->keys[DEGREE / 2];
        parent->size++;
    }

    void insertNonFull(BPlusNode* node, int key) {
        int i = node->size - 1;

        if (node->isLeaf) {
            while (i >= 0 && key < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
            node->size++;
        } else {
            while (i >= 0 && key < node->keys[i])
                i--;
            i++;
            if (node->children[i]->size == DEGREE) {
                splitChild(node, i);
                if (key > node->keys[i])
                    i++;
            }
            insertNonFull(node->children[i], key);
        }
    }

    bool searchRecursive(BPlusNode* node, int key) {
        int i = 0;
        while (i < node->size && key > node->keys[i])
            i++;

        if (i < node->size && key == node->keys[i])
            return true;

        if (node->isLeaf)
            return false;

        return searchRecursive(node->children[i], key);
    }
};

// -------------------- Query Planner --------------------
struct Query {
    std::string sql;
    int cost; // Lower cost = higher priority

    bool operator<(const Query& other) const {
        return cost > other.cost; // for min-heap behavior
    }
};

void simulateQueryPlanner() {
    std::priority_queue<Query> planner;
    planner.push({"SELECT * FROM users WHERE id = 5", 2});
    planner.push({"SELECT COUNT(*) FROM orders", 3});
    planner.push({"SELECT * FROM logs", 5});
    planner.push({"SELECT * FROM products WHERE price > 100", 1});

    std::cout << "\n[Query Execution Order by Cost]\n";
    while (!planner.empty()) {
        Query q = planner.top();
        planner.pop();
        std::cout << "Executing: " << q.sql << " (Cost: " << q.cost << ")\n";
    }
}

// -------------------- Main --------------------
int main() {
    std::cout << "=== Mini DB Query Engine ===\n";

    BPlusTree tree;
    std::vector<int> data = {10, 20, 5, 15, 25};

    std::cout << "\nInserting into B+ Tree:\n";
    for (int val : data) {
        std::cout << "Insert: " << val << "\n";
        tree.insert(val);
    }

    std::cout << "\nSearching B+ Tree:\n";
    for (int key : {15, 30}) {
        std::cout << "Search " << key << ": " << (tree.search(key) ? "Found" : "Not Found") << "\n";
    }

    simulateQueryPlanner();

    return 0;
}
