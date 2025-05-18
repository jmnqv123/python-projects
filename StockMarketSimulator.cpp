#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdlib>   // for rand()
#include <ctime>     // for time()
#include <thread>    // for sleep_for
#include <chrono>    // for chrono literals
using namespace std;

class StockMarket {
private:
    unordered_map<string, double> stockPrices;
    unordered_map<string, int> portfolio;
    double cash;

public:
    StockMarket() {
        // Initialize some stocks with prices
        stockPrices["AAPL"] = 150.0;
        stockPrices["GOOG"] = 2800.0;
        stockPrices["TSLA"] = 700.0;
        stockPrices["AMZN"] = 3300.0;
        cash = 10000.0; // Starting cash
        srand(time(0)); // Seed RNG
    }

    void simulatePriceChanges() {
        for (auto &stock : stockPrices) {
            double change = ((rand() % 2001) - 1000) / 100.0; // -10.00 to +10.00
            stock.second += change;
            if (stock.second < 1) stock.second = 1; // price can't go below 1
        }
    }

    void displayMarket() {
        cout << "\n=== Stock Market Prices ===\n";
        for (const auto &stock : stockPrices) {
            cout << stock.first << ": $" << stock.second << endl;
        }
        cout << "Cash Available: $" << cash << "\n";
    }

    void displayPortfolio() {
        cout << "\n=== Your Portfolio ===\n";
        if (portfolio.empty()) {
            cout << "No stocks owned yet.\n";
            return;
        }
        double totalValue = cash;
        for (const auto &item : portfolio) {
            double value = item.second * stockPrices[item.first];
            totalValue += value;
            cout << item.first << ": " << item.second << " shares (Value: $" << value << ")\n";
        }
        cout << "Total Portfolio Value (including cash): $" << totalValue << endl;
    }

    void buyStock(const string &symbol, int quantity) {
        if (stockPrices.find(symbol) == stockPrices.end()) {
            cout << "Stock symbol not found.\n";
            return;
        }
        double cost = stockPrices[symbol] * quantity;
        if (cost > cash) {
            cout << "Not enough cash to buy.\n";
            return;
        }
        cash -= cost;
        portfolio[symbol] += quantity;
        cout << "Bought " << quantity << " shares of " << symbol << ".\n";
    }

    void sellStock(const string &symbol, int quantity) {
        if (portfolio.find(symbol) == portfolio.end() || portfolio[symbol] < quantity) {
            cout << "You don't own that many shares.\n";
            return;
        }
        double revenue = stockPrices[symbol] * quantity;
        portfolio[symbol] -= quantity;
        if (portfolio[symbol] == 0) {
            portfolio.erase(symbol);
        }
        cash += revenue;
        cout << "Sold " << quantity << " shares of " << symbol << ".\n";
    }
};

int main() {
    StockMarket market;
    int choice;

    while (true) {
        market.simulatePriceChanges();
        market.displayMarket();
        market.displayPortfolio();

        cout << "\nOptions:\n1. Buy Stock\n2. Sell Stock\n3. Wait (Update Prices)\n4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string symbol;
            int qty;
            cout << "Enter stock symbol to buy: ";
            cin >> symbol;
            cout << "Enter quantity: ";
            cin >> qty;
            market.buyStock(symbol, qty);
        }
        else if (choice == 2) {
            string symbol;
            int qty;
            cout << "Enter stock symbol to sell: ";
            cin >> symbol;
            cout << "Enter quantity: ";
            cin >> qty;
            market.sellStock(symbol, qty);
        }
        else if (choice == 3) {
            cout << "Waiting for price updates...\n";
            this_thread::sleep_for(chrono::seconds(3));  // Pause 3 seconds to simulate wait
        }
        else {
            cout << "Exiting...\n";
            break;
        }
    }

    return 0;
}
