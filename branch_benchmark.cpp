#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

const int N = 100000000;

int predictable_branch() {
    int total = 0;
    for (int i = 0; i < N; ++i) {
        if (i < N) { // Always true
            total++;
        }
    }
    return total;
}

int unpredictable_branch() {
    int total = 0;
    for (int i = 0; i < N; ++i) {
        if (rand() % 2 == 0) { // Random: hard to predict
            total++;
        }
    }
    return total;
}

int main() {
    srand(time(0));

    cout << "Running predictable branch test...\n";
    auto start = high_resolution_clock::now();
    int predictable = predictable_branch();
    auto end = high_resolution_clock::now();
    auto predictable_time = duration_cast<milliseconds>(end - start).count();
    cout << "Predictable total: " << predictable << ", Time: " << predictable_time << " ms\n";

    cout << "Running unpredictable branch test...\n";
    start = high_resolution_clock::now();
    int unpredictable = unpredictable_branch();
    end = high_resolution_clock::now();
    auto unpredictable_time = duration_cast<milliseconds>(end - start).count();
    cout << "Unpredictable total: " << unpredictable << ", Time: " << unpredictable_time << " ms\n";

    return 0;
}

