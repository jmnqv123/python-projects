#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <memory>

struct Node {
    char ch;
    int freq;
    Node *left, *right;
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    Node(Node* l, Node* r) : ch('\0'), freq(l->freq + r->freq), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// Recursively build Huffman codes
void build_codes(Node* root, const std::string& code,
                 std::unordered_map<char, std::string>& huff_codes) {
    if (!root) return;
    if (!root->left && !root->right) {
        huff_codes[root->ch] = code;
    }
    build_codes(root->left, code + "0", huff_codes);
    build_codes(root->right, code + "1", huff_codes);
}

// Encode input string to binary using Huffman codes
std::string encode(const std::string& input, std::unordered_map<char, std::string>& huff_codes) {
    std::string encoded;
    for (char c : input) {
        encoded += huff_codes[c];
    }
    return encoded;
}

// Decode binary string using Huffman tree
std::string decode(const std::string& encoded, Node* root) {
    std::string result;
    Node* curr = root;
    for (char bit : encoded) {
        curr = (bit == '0') ? curr->left : curr->right;
        if (!curr->left && !curr->right) {
            result += curr->ch;
            curr = root;
        }
    }
    return result;
}

int main() {
    std::string input = "entropy entropy entropy compression";

    // Step 1: Count frequencies
    std::unordered_map<char, int> freq;
    for (char c : input) freq[c]++;

    // Step 2: Build Huffman tree
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (auto& [ch, f] : freq) {
        pq.push(new Node(ch, f));
    }

    while (pq.size() > 1) {
        Node* l = pq.top(); pq.pop();
        Node* r = pq.top(); pq.pop();
        pq.push(new Node(l, r));
    }
    Node* root = pq.top();

    // Step 3: Generate codes
    std::unordered_map<char, std::string> huff_codes;
    build_codes(root, "", huff_codes);

    // Step 4: Encode and decode
    std::string encoded = encode(input, huff_codes);
    std::string decoded = decode(encoded, root);

    std::cout << "Original: " << input << "\n";
    std::cout << "Encoded: " << encoded << "\n";
    std::cout << "Decoded: " << decoded << "\n";

    return 0;
}
