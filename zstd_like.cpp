#include <iostream>
#include <string>
#include <vector>
#include <sstream>

struct Token {
    int offset;
    int length;
    char next_char;
};

// Simple dictionary-based compression
std::vector<Token> compress(const std::string& input, int window_size = 16) {
    std::vector<Token> tokens;

    for (size_t i = 0; i < input.size();) {
        int best_offset = 0, best_length = 0;

        // Look for match in sliding window
        for (int offset = 1; offset <= std::min((int)i, window_size); ++offset) {
            int match_len = 0;
            while (i + match_len < input.size() &&
                   input[i - offset + match_len] == input[i + match_len]) {
                match_len++;
            }

            if (match_len > best_length) {
                best_offset = offset;
                best_length = match_len;
            }
        }

        char next_char = (i + best_length < input.size()) ? input[i + best_length] : '\0';
        tokens.push_back({best_offset, best_length, next_char});
        i += best_length + 1;
    }

    return tokens;
}

// Decompress back to original string
std::string decompress(const std::vector<Token>& tokens) {
    std::string result;

    for (const auto& token : tokens) {
        int start = result.size() - token.offset;
        for (int i = 0; i < token.length; ++i) {
            result += result[start + i];
        }
        if (token.next_char != '\0') result += token.next_char;
    }

    return result;
}

int main() {
    std::string input = "abcabcabcabcxyzxyzxyzxyz";
    std::cout << "Original: " << input << "\n";

    auto tokens = compress(input);
    std::cout << "Compressed tokens: \n";
    for (auto& t : tokens) {
        std::cout << "[offset=" << t.offset << ", len=" << t.length << ", next='" << t.next_char << "']\n";
    }

    std::string restored = decompress(tokens);
    std::cout << "Decompressed: " << restored << "\n";

    return 0;
}

