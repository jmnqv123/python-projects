#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

struct VectorClock {
    std::map<std::string, int> clock;

    void tick(const std::string& node) {
        clock[node]++;
    }

    void update(const VectorClock& other) {
        for (const auto& [k, v] : other.clock) {
            clock[k] = std::max(clock[k], v);
        }
    }

    std::string to_string() const {
        std::string s = "{ ";
        for (const auto& [k, v] : clock)
            s += k + ":" + std::to_string(v) + " ";
        return s + "}";
    }
};

struct Message {
    std::string sender;
    std::string text;
    VectorClock vc;
};

class ChatNode {
public:
    std::string name;
    VectorClock vc;
    std::vector<Message>& log;

    ChatNode(const std::string& name, std::vector<Message>& shared_log)
        : name(name), log(shared_log) {}

    void send(const std::string& text) {
        vc.tick(name);
        log.push_back({name, text, vc});
    }
};

void print_chat_log(const std::vector<Message>& log) {
    std::cout << "\n=== Chat Log ===\n";
    for (const auto& msg : log) {
        std::cout << msg.sender << ": " << msg.text
                  << " | VC: " << msg.vc.to_string() << "\n";
    }
    std::cout << "================\n";
}

int main() {
    std::vector<Message> chat_log;
    ChatNode nodeA("A", chat_log);
    ChatNode nodeB("B", chat_log);

    std::string input;
    while (true) {
        std::cout << "\nChoose sender (A/B) or Q to quit: ";
        std::getline(std::cin, input);

        if (input == "Q" || input == "q") break;
        if (input != "A" && input != "B") {
            std::cout << "Invalid sender.\n";
            continue;
        }

        std::string msg;
        std::cout << "Enter message: ";
        std::getline(std::cin, msg);

        if (input == "A")
            nodeA.send(msg);
        else
            nodeB.send(msg);

        print_chat_log(chat_log);
    }

    std::cout << "Exiting chat...\n";
    return 0;
}

