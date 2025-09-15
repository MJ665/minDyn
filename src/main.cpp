#include "Lexer.h"
#include "Parser.h"
#include "JIT.h"
#include <iostream>
#include <fstream>
#include <sstream>

// The main driver for the compiler.
void run(const std::string& source, bool enableJIT) {
    Lexer L(source);
    Parser P(L);
    
    // Create the JIT engine. It will manage both JIT and interpreter paths.
    auto Jit = std::make_unique<MinDynJIT>();
    
    P.getNextToken(); // Prime the parser

    while (true) {
        switch (P.CurTok) {
        case tok_eof:
            return;
        case ';': // ignore top-level semicolons.
            P.getNextToken();
            break;
        case tok_def:
            if (auto FnAST = P.ParseDefinition()) {
                fprintf(stderr, "Parsed a function definition: %s\n", FnAST->Proto->getName().c_str());
                Jit->addFunction(std::move(FnAST));
            } else {
                // Skip token for error recovery.
                P.getNextToken();
            }
            break;
        default:
            // It's a top-level expression. We need to execute it.
            if (auto ExprAST = P.ParseTopLevelExpr()) {
                if (enableJIT) {
                    std::string anonFuncName = ExprAST->Proto->getName();
                    Jit->addFunction(std::move(ExprAST));
                    double result = Jit->execute(anonFuncName);
                    fprintf(stdout, "Evaluated to: %f\n", result);
                } else {
                    fprintf(stderr, "Executing top-level expression via interpreter...\n");
                    double result = Jit->interpret(ExprAST->Proto->getName());
                    fprintf(stdout, "Evaluated to: %f\n", result);
                }
            } else {
                // Skip token for error recovery.
                P.getNextToken();
            }
            break;
        }
    }
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: mindyn [--jit] <filename>" << std::endl;
        return 1;
    }

    bool jitEnabled = false;
    std::string filename;

    if (argc == 3 && std::string(argv[1]) == "--jit") {
        jitEnabled = true;
        filename = argv[2];
    } else if (argc == 2) {
        filename = argv[1];
    } else {
        std::cerr << "Usage: mindyn [--jit] <filename>" << std::endl;
        return 1;
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    run(source, jitEnabled);

    return 0;
}
