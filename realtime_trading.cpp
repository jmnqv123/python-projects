#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include <string>

using namespace std;

struct Order {
    string type;  // "BUY" or "SELL"
    int quantity;
    float price;
};

const int MAX_ORDERS = 1000;
Order orderQueue[MAX_ORDERS];
atomic<int> head(0);
atomic<int> tail(0);

bool enqueue(const Order& order) {
    int current_tail = tail.load();
    int next_tail = (current_tail + 1) % MAX_ORDERS;
    if (next_tail == head.load()) return false; // Queue full
    orderQueue[current_tail] = order;
    tail.store(next_tail);
    return true;
}

bool dequeue(Order& order) {
    int current_head = head.load();
    if (current_head == tail.load()) return false; // Queue empty
    order = orderQueue[current_head];
    head.store((current_head + 1) % MAX_ORDERS);
    return true;
}

void producer() {
    int count = 0;
    while (count < 50) {
        Order order;
        order.type = (count % 2 == 0) ? "BUY" : "SELL";
        order.quantity = 1 + rand() % 100;
        order.price = 100.0f + static_cast<float>(rand() % 100);
        if (enqueue(order)) {
            cout << "Produced: " << order.type << " " << order.quantity << " @ $" << order.price << endl;
            count++;
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void consumer() {
    while (true) {
        Order order;
        if (dequeue(order)) {
            cout << "Executed: " << order.type << " " << order.quantity << " @ $" << order.price << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(150));
    }
}

int main() {
    thread t1(producer);
    thread t2(consumer);

    t1.join();
    // Let consumer run for some time after producer ends
    this_thread::sleep_for(chrono::seconds(10));
    return 0;
}
