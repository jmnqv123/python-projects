#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>

class TieredCompaction {
    std::vector<std::vector<int>> levels;
    size_t max_level_size;
    int max_levels;

public:
    TieredCompaction(size_t level_size = 4, int levels = 3)
        : max_level_size(level_size), max_levels(levels) {
        this->levels.resize(levels);
    }

    void add_data(const std::vector<int>& data) {
        std::vector<int> sorted_data = data;
        std::sort(sorted_data.begin(), sorted_data.end());
        levels[0].insert(levels[0].end(), sorted_data.begin(), sorted_data.end());
        maybe_compact(0);
    }

    void maybe_compact(int level) {
        if (level >= max_levels - 1) return;

        if (levels[level].size() > max_level_size) {
            std::cout << "Compacting Level " << level << " into Level " << (level + 1) << "\n";
            std::vector<int>& current = levels[level];
            std::vector<int>& next = levels[level + 1];

            std::vector<int> merged;
            merged.reserve(current.size() + next.size());
            std::merge(current.begin(), current.end(), next.begin(), next.end(), std::back_inserter(merged));

            current.clear();
            next = merged;
            maybe_compact(level + 1);
        }
    }

    void print_levels() const {
        for (size_t i = 0; i < levels.size(); ++i) {
            std::cout << "Level " << i << ": ";
            for (int val : levels[i]) std::cout << val << " ";
            std::cout << "\n";
        }
    }
};

std::vector<int> generate_data(int count, int min = 0, int max = 100) {
    std::vector<int> data(count);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    for (int& d : data) d = dis(gen);
    return data;
}

int main() {
    TieredCompaction tc;

    for (int i = 0; i < 5; ++i) {
        auto data = generate_data(3);
        std::cout << "Adding batch: ";
        for (int d : data) std::cout << d << " ";
        std::cout << "\n";
        tc.add_data(data);
        tc.print_levels();
        std::cout << "-------------------\n";
    }

    return 0;
}
