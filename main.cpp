#include <iostream>

int main() {
    int* ptr = nullptr; // ptr is a null pointer, pointing to nothing

    std::cout << "Attempting to dereference a null pointer..." << std::endl;

    // This line causes Undefined Behavior (and likely a crash!)
    *ptr = 42; // Trying to put the value 42 into the "house" at "no address"

    std::cout << "This line might not be reached if the program crashes." << std::endl;

    return 0;
}