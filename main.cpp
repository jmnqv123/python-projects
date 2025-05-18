#include <iostream>
#include <memory>

template<typename T>
class MyVector {
private:
    T* data;
    size_t cap;
    size_t len;

    void reallocate(size_t newCap) {
        T* newData = new T[newCap];
        for (size_t i = 0; i < len; ++i)
            newData[i] = data[i];
        delete[] data;
        data = newData;
        cap = newCap;
    }

public:
    MyVector() : data(nullptr), cap(0), len(0) {}

    ~MyVector() {
        delete[] data;
    }

    void push_back(const T& val) {
        if (len == cap) {
            size_t newCap = (cap == 0) ? 1 : cap * 2;
            reallocate(newCap);
        }
        data[len++] = val;
    }

    size_t size() const {
        return len;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    // Nested iterator class
    class Iterator {
    private:
        T* ptr;
    public:
        Iterator(T* p) : ptr(p) {}
        Iterator operator++() { ++ptr; return *this; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
        T& operator*() const { return *ptr; }
    };

    Iterator begin() { return Iterator(data); }
    Iterator end() { return Iterator(data + len); }
};

// Usage example
int main() {
    MyVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    std::cout << "MyVector contents:\n";
    for (auto val : vec) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    std::cout << "Access by index: " << vec[1] << "\n";
    return 0;
}

    