// numa_scheduler.cpp
#include <iostream>
#include <thread>
#include <vector>
#include <windows.h>

void simulate_task(int thread_id, DWORD_PTR affinity_mask) {
    HANDLE this_thread = GetCurrentThread();
    SetThreadAffinityMask(this_thread, affinity_mask);

    std::cout << "Thread " << thread_id << " running on NUMA node (mask: " << affinity_mask << ")\n";
    for (volatile int i = 0; i < 100000000; ++i);  // Simulate CPU load
    std::cout << "Thread " << thread_id << " finished.\n";
}

int main() {
    const int num_threads = 4;
    std::vector<std::thread> threads;
    DWORD_PTR numa_masks[] = { 1, 2, 1, 2 };

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(simulate_task, i, numa_masks[i]);
    }

    for (auto& t : threads)
        t.join();

    std::cout << "All threads completed.\n";
    return 0;
}
