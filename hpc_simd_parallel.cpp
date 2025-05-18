// hpc_simd_parallel.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <chrono>
#include <immintrin.h> // For AVX intrinsics

void simd_add(const float* a, const float* b, float* result, size_t size) {
    size_t i = 0;
    for (; i + 7 < size; i += 8) {
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vb = _mm256_loadu_ps(&b[i]);
        __m256 vr = _mm256_add_ps(va, vb);
        _mm256_storeu_ps(&result[i], vr);
    }
    for (; i < size; ++i) {
        result[i] = a[i] + b[i];
    }
}

int main() {
    const size_t SIZE = 1 << 20; // 1 million elements
    std::vector<float> a(SIZE, 1.0f);
    std::vector<float> b(SIZE, 2.0f);
    std::vector<float> result(SIZE);
    std::vector<float> result_parallel(SIZE);

    // SIMD version
    auto start = std::chrono::high_resolution_clock::now();
    simd_add(a.data(), b.data(), result.data(), SIZE);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "[SIMD] Time: " << std::chrono::duration<double>(end - start).count() << "s\n";

    // Parallel STL version
    start = std::chrono::high_resolution_clock::now();
    std::transform(std::execution::par, a.begin(), a.end(), b.begin(), result_parallel.begin(),
                   [](float x, float y) { return x + y; });
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[Parallel STL] Time: " << std::chrono::duration<double>(end - start).count() << "s\n";

    // Sanity check
    std::cout << "Sample result[0]: SIMD=" << result[0] << " | Parallel=" << result_parallel[0] << std::endl;

    return 0;
}
