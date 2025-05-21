#include <coroutine>
#include <iostream>
#include <optional>

// Coroutine Generator Type
template<typename T>
struct Generator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        T current_value;

        auto get_return_object() {
            return Generator{handle_type::from_promise(*this)};
        }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void return_void() {}
        auto yield_value(T value) {
            current_value = value;
            return std::suspend_always{};
        }
        void unhandled_exception() {
            std::exit(1);
        }
    };

    handle_type coro;

    Generator(handle_type h) : coro(h) {}
    ~Generator() { if (coro) coro.destroy(); }

    bool next() {
        if (!coro.done()) {
            coro.resume();
        }
        return !coro.done();
    }

    T value() const {
        return coro.promise().current_value;
    }
};

// Coroutine function
Generator<int> counter(int max) {
    for (int i = 0; i < max; ++i) {
        std::cout << "[Coroutine] Yielding: " << i << std::endl;
        co_yield i;
    }
}

int main() {
    std::cout << "[Main] Starting coroutine...\n";

    auto gen = counter(5);
    while (gen.next()) {
        std::cout << "[Main] Got: " << gen.value() << "\n";
    }

    std::cout << "[Main] Coroutine finished.\n";
    return 0;
}
