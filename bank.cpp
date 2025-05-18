#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

// Base class
class Account {
protected:
    string owner;
    double balance;
    vector<string> transactionLog;

    string currentTime() {
        time_t now = time(0);
        char* dt = ctime(&now);
        return string(dt);
    }

public:
    Account(string name, double initial = 0) : owner(name), balance(initial) {
        log("Account created with balance: " + to_string(initial));
    }

    virtual ~Account() {
        log("Account closed");
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "❌ Invalid deposit amount.\n";
            return;
        }
        balance += amount;
        log("Deposited: " + to_string(amount));
    }

    virtual void withdraw(double amount) {
        if (amount <= 0 || amount > balance) {
            cout << "❌ Invalid or insufficient balance.\n";
            return;
        }
        balance -= amount;
        log("Withdrew: " + to_string(amount));
    }

    double getBalance() const {
        return balance;
    }

    void log(const string& entry) {
        transactionLog.push_back(currentTime() + ": " + entry);
    }

    void showLog() const {
        cout << "\n📒 Transaction Log:\n";
        for (const string& entry : transactionLog) {
            cout << entry;
        }
    }

    virtual void showDetails() const {
        cout << "\n👤 Owner: " << owner << "\n💰 Balance: $" << balance << endl;
    }
};

// Derived class
class SavingsAccount : public Account {
    double interestRate;

public:
    SavingsAccount(string name, double initial, double rate)
        : Account(name, initial), interestRate(rate) {
        log("Savings account created with interest rate: " + to_string(rate));
    }

    void applyInterest() {
        double interest = balance * interestRate;
        deposit(interest);
        log("Interest applied: " + to_string(interest));
    }

    void showDetails() const override {
        Account::showDetails();
        cout << "🏦 Interest rate: " << interestRate * 100 << "%" << endl;
    }
};

// MAIN
int main() {
    SavingsAccount acc("Alice", 500.0, 0.05);

    acc.deposit(200.0);
    acc.withdraw(100.0);
    acc.applyInterest();
    acc.showDetails();
    acc.showLog();

    return 0;
}
