#include <iostream>
#include <vector>
#include <string>

// Simple Run-Length Encoding compression
std::string compressRLE(const std::string& input) {
    std::string compressed;
    int count = 1;

    for (size_t i = 1; i <= input.length(); ++i) {
        if (i < input.length() && input[i] == input[i - 1]) {
            count++;
        } else {
            compressed += input[i - 1];
            compressed += std::to_string(count);
            count = 1;
        }
    }

    return compressed;
}

// RLE decompression
std::string decompressRLE(const std::string& compressed) {
    std::string decompressed;
    for (size_t i = 0; i < compressed.length(); ++i) {
        char c = compressed[i];
        std::string countStr;

        // Read number after character
        while (i + 1 < compressed.length() && isdigit(compressed[i + 1])) {
            countStr += compressed[++i];
        }

        int count = std::stoi(countStr);
        decompressed.append(count, c);
    }

    return decompressed;
}

int main() {
    std::string text = "aaabbbccccccddddddddd";
    std::string compressed = compressRLE(text);
    std::string decompressed = decompressRLE(compressed);

    std::cout << "Original:      " << text << "\n";
    std::cout << "Compressed:    " << compressed << "\n";
    std::cout << "Decompressed:  " << decompressed << "\n";

    return 0;
}
