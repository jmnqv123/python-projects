#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <vector>
#include <chrono>

class Message {
public:
    int sender_pid;
    std::string content;
};

class Process {
private:
    int pid;
    std::queue<Message> inbox;
    std::mutex mtx;
    std::condition_variable cv;

public:
    Process(int id) : pid(id) {}

    int get_pid() const { return pid; }

    void send(Process& target, const std::string& msg) {
        std::lock_guard<std::mutex> lock(target.mtx);
        target.inbox.push({pid, msg});
        target.cv.notify_one();
    }

    void run() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            if (cv.wait_for(lock, std::chrono::seconds(2), [&]() { return !inbox.empty(); })) {
                Message msg = inbox.front();
                inbox.pop();
                std::cout << "[Process " << pid << "] Received message from " << msg.sender_pid
                          << ": " << msg.content << std::endl;
            } else {
                std::cout << "[Process " << pid << "] No message. Still alive.\n";
            }
        }
    }
};

// Simulate kernel that starts multiple processes (threads)
int main() {
    Process p1(1), p2(2), p3(3);

    std::thread t1(&Process::run, &p1);
    std::thread t2(&Process::run, &p2);
    std::thread t3(&Process::run, &p3);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    p1.send(p2, "Hi from P1");
    p2.send(p3, "Hi from P2");
    p3.send(p1, "Hi from P3");

    // Let threads run a bit
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // In real OS, you'd have process termination, join, etc.
    std::cout << "Simulation complete. Force stopping (use Ctrl+C if running indefinitely).\n";

    // No clean exit to simulate indefinite running like real multitasking.
    t1.detach();
    t2.detach();
    t3.detach();

    return 0;
}
