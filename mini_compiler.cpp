#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <cctype>
using namespace std;

// === Token Definition ===
enum class TokenType { Number, Plus, Star, End };

struct Token {
    TokenType type;
    int value;
};

// === Lexer ===
class Lexer {
public:
    Lexer(const string& src) : input(src), pos(0) {}
    Token getNextToken() {
        while (pos < input.size() && isspace(input[pos])) pos++;

        if (pos >= input.size()) return {TokenType::End, 0};
        char ch = input[pos];

        if (isdigit(ch)) {
            int val = 0;
            while (pos < input.size() && isdigit(input[pos])) {
                val = val * 10 + (input[pos++] - '0');
            }
            return {TokenType::Number, val};
        } else if (ch == '+') {
            pos++;
            return {TokenType::Plus, 0};
        } else if (ch == '*') {
            pos++;
            return {TokenType::Star, 0};
        }

        throw runtime_error("Unknown character!");
    }

private:
    string input;
    size_t pos;
};

// === AST Node Base ===
class Expr {
public:
    virtual int eval() = 0;
    virtual ~Expr() = default;
};

class NumberExpr : public Expr {
    int value;
public:
    NumberExpr(int v) : value(v) {}
    int eval() override { return value; }
};

class BinaryExpr : public Expr {
    char op;
    unique_ptr<Expr> left, right;
public:
    BinaryExpr(char o, unique_ptr<Expr> l, unique_ptr<Expr> r)
        : op(o), left(move(l)), right(move(r)) {}
    int eval() override {
        if (op == '+') return left->eval() + right->eval();
        if (op == '*') return left->eval() * right->eval();
        throw runtime_error("Unknown op");
    }
};

// === Parser ===
class Parser {
public:
    Parser(Lexer& lex) : lexer(lex), current(lexer.getNextToken()) {}

    unique_ptr<Expr> parseExpr() {
        auto left = parseTerm();
        while (current.type == TokenType::Plus) {
            consume(TokenType::Plus);
            auto right = parseTerm();
            left = make_unique<BinaryExpr>('+', move(left), move(right));
        }
        return left;
    }

private:
    Token current;
    Lexer& lexer;

    void consume(TokenType expected) {
        if (current.type != expected) throw runtime_error("Unexpected token!");
        current = lexer.getNextToken();
    }

    unique_ptr<Expr> parseTerm() {
        auto left = parseFactor();
        while (current.type == TokenType::Star) {
            consume(TokenType::Star);
            auto right = parseFactor();
            left = make_unique<BinaryExpr>('*', move(left), move(right));
        }
        return left;
    }

    unique_ptr<Expr> parseFactor() {
        if (current.type == TokenType::Number) {
            int val = current.value;
            consume(TokenType::Number);
            return make_unique<NumberExpr>(val);
        }
        throw runtime_error("Expected number");
    }
};

// === Main ===
int main() {
    cout << "Enter arithmetic expression (e.g., 2 + 3 * 4): ";
    string input;
    getline(cin, input);

    try {
        Lexer lexer(input);
        Parser parser(lexer);
        auto tree = parser.parseExpr();
        cout << "Result = " << tree->eval() << endl;
    } catch (exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
