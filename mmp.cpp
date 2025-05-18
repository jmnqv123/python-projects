#include <iostream>
using namespace std;

int main() {
    // Step 1: Declare a pointer to int
    int* ptr = nullptr;

    // Step 2: Allocate memory dynamically for an integer
    ptr = new int; // Now ptr points to a memory block that can store an int

    // Step 3: Assign a value using the pointer
    *ptr = 42;

    // Step 4: Display the value and address
    cout << "Value stored at ptr: " << *ptr << endl;
    cout << "Address stored in ptr: " << ptr << endl;

    // Step 5: Deallocate the memory
    delete ptr;
    ptr = nullptr; // Good practice to avoid dangling pointer

    return 0;
}
