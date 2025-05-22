#include <iostream>
#include <vector>
#include <string>
#include <random>

enum Vote { VOTE_COMMIT, VOTE_ABORT };
enum State { INIT, PREPARED, COMMITTED, ABORTED };

class Participant {
    std::string name;
    State state;

public:
    Participant(const std::string& n) : name(n), state(INIT) {}

    Vote prepare() {
        // Simulate success or failure
        bool success = (rand() % 100) < 80;  // 80% chance to commit
        state = success ? PREPARED : ABORTED;
        std::cout << name << " votes: " << (success ? "COMMIT" : "ABORT") << "\n";
        return success ? VOTE_COMMIT : VOTE_ABORT;
    }

    void commit() {
        if (state == PREPARED) {
            state = COMMITTED;
            std::cout << name << " committed.\n";
        }
    }

    void abort() {
        state = ABORTED;
        std::cout << name << " aborted.\n";
    }
};

class TransactionManager {
    std::vector<Participant> participants;

public:
    void add_participant(const std::string& name) {
        participants.emplace_back(name);
    }

    void run_transaction() {
        std::cout << "=== Starting Distributed Transaction ===\n";
        bool all_commit = true;

        // Phase 1: Prepare
        for (auto& p : participants) {
            if (p.prepare() == VOTE_ABORT) {
                all_commit = false;
            }
        }

        // Phase 2: Commit or Abort
        for (auto& p : participants) {
            if (all_commit)
                p.commit();
            else
                p.abort();
        }

        std::cout << "=== Transaction " << (all_commit ? "COMMITTED" : "ABORTED") << " ===\n";
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    TransactionManager tm;
    tm.add_participant("Node-A");
    tm.add_participant("Node-B");
    tm.add_participant("Node-C");

    tm.run_transaction();
    return 0;
}
