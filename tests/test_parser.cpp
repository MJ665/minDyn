#include "Parser.h"
#include "Lexer.h"
#include <cassert>
#include <iostream>
#include <string>

// A simple assertion macro for testing
#define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            exit(1); \
        } \
    } while (false)

void test_parse_number() {
    std::cout << "  Running test: test_parse_number..." << std::endl;
    std::string source = "42;";
    Lexer L(source);
    Parser P(L);
    P.getNextToken();
    auto expr = P.ParseTopLevelExpr();
    
    ASSERT(expr != nullptr, "Parser returned null for a valid number expression.");
    
    // We would need to add RTTI or visitor pattern to inspect the AST node type.
    // For now, we just check that it parsed successfully.
    std::cout << "    ...PASSED\n";
}

void test_parse_definition() {
    std::cout << "  Running test: test_parse_definition..." << std::endl;
    std::string source = "def foo(a b) a+b;";
    Lexer L(source);
    Parser P(L);
    P.getNextToken(); // prime
    auto def = P.ParseDefinition();

    ASSERT(def != nullptr, "Parser returned null for a valid function definition.");
    ASSERT(def->Proto->getName() == "foo", "Function name was not parsed correctly.");
    std::cout << "    ...PASSED\n";
}


void run_parser_tests() {
    std::cout << "--- Running Parser Tests ---\n";
    test_parse_number();
    test_parse_definition();
    std::cout << "---------------------------\n";
}
