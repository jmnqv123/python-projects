#include <iostream>
#include <vector>
#include <chrono>

constexpr int N = 512;       // Matrix size (NxN)
constexpr int BLOCK_SIZE = 32; // Tile size for blocking

using Matrix = std::vector<std::vector<float>>;

// Initialize matrix with constant
void init(Matrix &mat, float val) {
    for (auto &row : mat)
        std::fill(row.begin(), row.end(), val);
}

// Cache-optimized matrix multiplication (loop tiling)
void matmul_blocked(const Matrix &A, const Matrix &B, Matrix &C) {
    for (int ii = 0; ii < N; ii += BLOCK_SIZE) {
        for (int jj = 0; jj < N; jj += BLOCK_SIZE) {
            for (int kk = 0; kk < N; kk += BLOCK_SIZE) {
                for (int i = ii; i < std::min(ii + BLOCK_SIZE, N); ++i) {
                    for (int j = jj; j < std::min(jj + BLOCK_SIZE, N); ++j) {
                        float sum = C[i][j];
                        for (int k = kk; k < std::min(kk + BLOCK_SIZE, N); ++k) {
                            sum += A[i][k] * B[k][j];
                        }
                        C[i][j] = sum;
                    }
                }
            }
        }
    }
}

int main() {
    Matrix A(N, std::vector<float>(N));
    Matrix B(N, std::vector<float>(N));
    Matrix C(N, std::vector<float>(N, 0));

    init(A, 1.0f);
    init(B, 2.0f);

    auto start = std::chrono::high_resolution_clock::now();
    matmul_blocked(A, B, C);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Blocked matrix multiplication time: " << elapsed.count() << "s\n";
    std::cout << "C[0][0] = " << C[0][0] << "\n";

    return 0;
}
