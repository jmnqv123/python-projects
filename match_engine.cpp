#include <iostream>
#include <queue>
#include <chrono>

struct Order {
    int id;
    int quantity;
    double price;
    bool isBuy;
};

class MatchingEngine {
private:
    std::queue<Order> buyOrders;
    std::queue<Order> sellOrders;

public:
    void addOrder(const Order& order) {
        auto start = std::chrono::high_resolution_clock::now();

        if (order.isBuy) {
            if (!sellOrders.empty() && sellOrders.front().price <= order.price) {
                std::cout << "✅ Matched buy order " << order.id << " with sell order " << sellOrders.front().id << "\n";
                sellOrders.pop();
            } else {
                buyOrders.push(order);
                std::cout << "📥 Buy order queued\n";
            }
        } else {
            if (!buyOrders.empty() && buyOrders.front().price >= order.price) {
                std::cout << "✅ Matched sell order " << order.id << " with buy order " << buyOrders.front().id << "\n";
                buyOrders.pop();
            } else {
                sellOrders.push(order);
                std::cout << "📥 Sell order queued\n";
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << "⏱️ Match latency: " << ns << " ns\n";
    }
};

int main() {
    MatchingEngine engine;

    Order order1 = {1, 100, 101.5, true};   // Buy order
    Order order2 = {2, 100, 101.0, false};  // Sell order

    engine.addOrder(order1);
    engine.addOrder(order2);

    return 0;
}
