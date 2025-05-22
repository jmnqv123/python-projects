#include <iostream>
#include <fstream>
#include <map>
#include <string>

class LSMTree {
private:
    std::map<std::string, std::string> memtable;
    std::string sstable_file;
    size_t memtable_limit;

    void flush_to_disk() {
        std::ofstream out(sstable_file, std::ios::app);
        for (const auto& [key, value] : memtable) {
            out << key << " " << value << "\n";
        }
        memtable.clear();
    }

    void load_from_disk() {
        std::ifstream in(sstable_file);
        std::string key, value;
        while (in >> key >> value) {
            memtable[key] = value;
        }
    }

public:
    LSMTree(const std::string& file, size_t limit = 4)
        : sstable_file(file), memtable_limit(limit) {
        load_from_disk();
    }

    void put(const std::string& key, const std::string& value) {
        memtable[key] = value;
        if (memtable.size() >= memtable_limit) {
            flush_to_disk();
        }
    }

    std::string get(const std::string& key) {
        if (memtable.count(key)) return memtable[key];

        std::ifstream in(sstable_file);
        std::string k, v;
        while (in >> k >> v) {
            if (k == key) return v;
        }
        return "";
    }

    void del(const std::string& key) {
        put(key, "__TOMBSTONE__");
    }
};

int main() {
    LSMTree db("sstable.sst");

    db.put("a", "apple");
    db.put("b", "banana");
    db.put("c", "carrot");
    db.put("d", "date");

    std::cout << "Get a: " << db.get("a") << "\n";
    std::cout << "Get b: " << db.get("b") << "\n";

    db.del("b");

    std::cout << "Get b after delete: " << db.get("b") << "\n";

    return 0;
}
