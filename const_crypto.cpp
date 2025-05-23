#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

// Constant-time comparison
bool constant_time_compare(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    uint8_t result = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        result |= a[i] ^ b[i];
    }
    return result == 0;
}

// Simple XOR encryption (not secure, demo only)
std::string xor_encrypt(const std::string& data, const std::string& key) {
    std::string output = data;
    for (size_t i = 0; i < data.size(); ++i) {
        output[i] ^= key[i % key.size()];
    }
    return output;
}

int main() {
    std::string secret = "HelloWorld";
    std::string key = "K3y123";

    std::string encrypted = xor_encrypt(secret, key);
    std::string decrypted = xor_encrypt(encrypted, key); // XOR again to decrypt

    std::cout << "Encrypted (hex): ";
    for (unsigned char c : encrypted) {
        printf("%02X ", c);
    }
    std::cout << "\nDecrypted: " << decrypted << "\n";

    // Constant-time comparison test
    std::string input;
    std::cout << "Enter password to check: ";
    std::cin >> input;

    if (constant_time_compare(input, secret)) {
        std::cout << "Access granted.\n";
    } else {
        std::cout << "Access denied.\n";
    }

    return 0;
}
