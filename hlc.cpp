#include <iostream>
#include <chrono>
#include <algorithm>
#include <thread>

class HybridLogicalClock {
private:
    uint64_t last_physical;
    uint16_t logical;

    uint64_t now() const {
        using namespace std::chrono;
        return duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()).count();
    }

public:
    HybridLogicalClock() {
        last_physical = now();
        logical = 0;
    }

    struct Timestamp {
        uint64_t physical;
        uint16_t logical;

        void print(const std::string& label) const {
            std::cout << label << ": (" << physical << ", " << logical << ")\n";
        }
    };

    Timestamp get_time() {
        uint64_t current = now();
        if (current > last_physical) {
            last_physical = current;
            logical = 0;
        } else {
            logical++;
        }
        return { last_physical, logical };
    }

    Timestamp update(const Timestamp& received) {
        uint64_t local_now = now();
        last_physical = std::max({ received.physical, local_now, last_physical });
        if (last_physical == received.physical && last_physical == local_now) {
            logical = std::max(logical, received.logical) + 1;
        } else if (last_physical == received.physical) {
            logical = received.logical + 1;
        } else if (last_physical == local_now) {
            logical = logical + 1;
        } else {
            logical = 0;
        }
        return { last_physical, logical };
    }
};

int main() {
    HybridLogicalClock nodeA, nodeB;

    // Simulate local event on Node A
    auto tsA1 = nodeA.get_time();
    tsA1.print("Node A Event 1");

    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // simulate time

    // Simulate sending timestamp from A to B
    auto tsB1 = nodeB.update(tsA1);
    tsB1.print("Node B after receiving from A");

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Local event on Node B
    auto tsB2 = nodeB.get_time();
    tsB2.print("Node B Event 2");

    // Send timestamp from B to A
    auto tsA2 = nodeA.update(tsB2);
    tsA2.print("Node A after receiving from B");

    return 0;
}
