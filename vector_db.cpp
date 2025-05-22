#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <limits>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

using Vector = vector<float>;
using VectorStore = unordered_map<string, Vector>;

float euclidean_distance(const Vector& a, const Vector& b) {
    if (a.size() != b.size()) throw runtime_error("Dimension mismatch.");
    float sum = 0;
    for (size_t i = 0; i < a.size(); ++i)
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    return sqrt(sum);
}

class VectorDB {
    VectorStore store;

public:
    void insert(const string& name, const Vector& vec) {
        store[name] = vec;
        cout << "Inserted vector '" << name << "'\n";
    }

    string search_nearest(const Vector& query) {
        float min_dist = numeric_limits<float>::max();
        string closest;

        for (const auto& [name, vec] : store) {
            try {
                float dist = euclidean_distance(vec, query);
                if (dist < min_dist) {
                    min_dist = dist;
                    closest = name;
                }
            } catch (...) {
                continue;
            }
        }

        return closest.empty() ? "No match found." : closest;
    }

    void list_all() {
        for (const auto& [name, vec] : store) {
            cout << name << ": [ ";
            for (float val : vec)
                cout << fixed << setprecision(2) << val << " ";
            cout << "]\n";
        }
    }
};

// Helper: convert string to vector<float>
Vector parse_vector(const string& input) {
    stringstream ss(input);
    float num;
    Vector vec;
    while (ss >> num) vec.push_back(num);
    return vec;
}

int main() {
    VectorDB db;
    string cmd;

    cout << "VectorDB CLI - Commands: insert, search, list, exit\n";

    while (true) {
        cout << "\n> ";
        getline(cin, cmd);
        stringstream ss(cmd);
        string action;
        ss >> action;

        if (action == "insert") {
            string name;
            ss >> name;
            string rest;
            getline(ss, rest);
            Vector vec = parse_vector(rest);
            db.insert(name, vec);
        }
        else if (action == "search") {
            string rest;
            getline(ss, rest);
            Vector query = parse_vector(rest);
            string result = db.search_nearest(query);
            cout << "Nearest vector: " << result << "\n";
        }
        else if (action == "list") {
            db.list_all();
        }
        else if (action == "exit") {
            break;
        }
        else {
            cout << "Unknown command.\n";
        }
    }

    return 0;
}
