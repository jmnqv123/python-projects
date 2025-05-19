#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <map>

class Message {
public:
    int sender_pid;
    std::string content;
};

class Process {
public:
    int pid;
    std::queue<Message> inbox;
    std::mutex mtx;
    std::condition_variable cv;

    Process(int pid_) : pid(pid_) {}

    void receive() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]() { return !inbox.empty(); });
        Message msg = inbox.front();
        inbox.pop();
        std::cout << "Process " << pid << " received message from " << msg.sender_pid << ": " << msg.content << std::endl;
    }

    void send(Process& target, const std::string& content) {
        std::lock_guard<std::mutex> lock(target.mtx);
        target.inbox.push({pid, content});
        target.cv.notify_one();
    }
};

// Simulated User-space Driver
class Driver {
public:
    void load() {
        std::cout << "User-space driver loaded and running in user mode." << std::endl;
    }
};

int main() {
    Process p1(1);
    Process p2(2);

    std::thread t1([&]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        p1.send(p2, "Hello from Process 1");
    });

    std::thread t2([&]() {
        p2.receive();
    });

    Driver d;
    d.load();

    t1.join();
    t2.join();

    return 0;
}
