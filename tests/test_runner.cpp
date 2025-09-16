#include <iostream>

// This is the main entry point for the test executable.
// It will call test functions from other files.

// Declare the test functions from other files
void run_parser_tests();

int main(int argc, char **argv) {
    std::cout << "Running MinDyn tests...\n";
    
    run_parser_tests();
    
    std::cout << "All tests passed.\n";
    return 0;
}
