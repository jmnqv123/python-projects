#include <iostream>
#include <memory>
#include <unordered_set>

// Simple tracker for allocated objects
class GCTracker {
public:
    static std::unordered_set<void*> active;

    static void* track(void* ptr) {
        active.insert(ptr);
        std::cout << "Allocated: " << ptr << "\n";
        return ptr;
    }

    static void untrack(void* ptr) {
        active.erase(ptr);
        std::cout << "Deallocated: " << ptr << "\n";
    }

    static void report() {
        std::cout << "\n🧹 GC Report:\n";
        for (auto ptr : active)
            std::cout << "Leaked: " << ptr << "\n";
        if (active.empty())
            std::cout << "No leaks detected.\n";
    }
};

std::unordered_set<void*> GCTracker::active;

// Custom deleter
struct GCDeleter {
    void operator()(int* ptr) const {
        GCTracker::untrack(ptr);
        delete ptr;
    }
};

int main() {
    {
        // Smart pointer with tracking
        std::shared_ptr<int> p1(static_cast<int*>(GCTracker::track(new int(42))), GCDeleter{});
        std::shared_ptr<int> p2(static_cast<int*>(GCTracker::track(new int(99))), GCDeleter{});
        std::cout << "p1: " << *p1 << ", p2: " << *p2 << "\n";
    } // p1 and p2 go out of scope — memory auto-freed

    // Report any leaks
    GCTracker::report();
    return 0;
}
