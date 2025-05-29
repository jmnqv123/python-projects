#include <iostream>
#include <array>
#include <string_view>
#include <cstdint>  // <-- Needed for uint32_t

// Compile-time string wrapper
template <size_t N>
struct ConstStr {
    std::array<char, N> data;

    constexpr ConstStr(const char (&str)[N]) {
        for (size_t i = 0; i < N; ++i)
            data[i] = str[i];
    }

    constexpr char operator[](size_t i) const {
        return data[i];
    }

    constexpr size_t size() const {
        return N;
    }

    constexpr std::string_view view() const {
        return std::string_view(data.data(), N - 1); // exclude null
    }
};

// Compile-time string reverse
template <ConstStr input>
constexpr auto reverse() {
    std::array<char, input.size()> result{};
    for (size_t i = 0; i < input.size() - 1; ++i) {
        result[i] = input[input.size() - 2 - i]; // reverse excluding null
    }
    result[input.size() - 1] = '\0'; // null-terminate
    return result;
}

// Compile-time FNV-1a hash
constexpr std::uint32_t fnv1a(const char* str, size_t len) {
    std::uint32_t hash = 0x811c9dc5;
    for (size_t i = 0; i < len; ++i) {
        hash ^= static_cast<std::uint32_t>(str[i]);
        hash *= 0x01000193;
    }
    return hash;
}

int main() {
    constexpr ConstStr<sizeof("compile-time")> original("compile-time");
    constexpr auto reversed = reverse<original>();
    constexpr auto hash_val = fnv1a(original.data.data(), original.size() - 1);

    std::cout << "Original: " << original.view() << "\n";
    std::cout << "Reversed: " << reversed.data() << "\n";
    std::cout << "FNV-1a Hash: " << hash_val << "\n";

    return 0;
}
