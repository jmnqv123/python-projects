#include <iostream>
#include <vector>
#include <chrono>

int main() {
    const int N = 10000000;
    std::vector<int> data(N, 1);

    // Introduce some zeroes to simulate branch unpredictability
    for (int i = 0; i < N; i += 100) {
        data[i] = 0;
    }

    // Simulate branch prediction success
    auto start_predicted = std::chrono::high_resolution_clock::now();
    int sum_predicted = 0;
    for (int i = 0; i < N; ++i) {
        if (data[i] != 0)  // mostly true, predictable branch
            sum_predicted += data[i];
    }
    auto end_predicted = std::chrono::high_resolution_clock::now();

    // Simulate branch prediction failure
    auto start_mispredict = std::chrono::high_resolution_clock::now();
    int sum_mispredict = 0;
    for (int i = 0; i < N; ++i) {
        if ((i % 2) == 0)  // unpredictable pattern
            sum_mispredict += data[i];
    }
    auto end_mispredict = std::chrono::high_resolution_clock::now();

    auto t1 = std::chrono::duration<double, std::milli>(end_predicted - start_predicted).count();
    auto t2 = std::chrono::duration<double, std::milli>(end_mispredict - start_mispredict).count();

    std::cout << "Sum (predicted): " << sum_predicted << " | Time: " << t1 << " ms\n";
    std::cout << "Sum (mispredict): " << sum_mispredict << " | Time: " << t2 << " ms\n";

    if (t2 > t1)
        std::cout << "Speculative execution penalty observed due to mispredicted branches.\n";
    else
        std::cout << "No significant penalty observed.\n";

    return 0;
}
