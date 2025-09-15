#pragma once

#include "AST.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include <map>
#include <string>
#include <memory>

class MinDynJIT {
private:
    std::unique_ptr<llvm::orc::LLJIT> TheJIT;

    // Profiler to count function calls
    std::map<std::string, int> FunctionCallCounts;
    const int JIT_THRESHOLD = 5; // JIT compile after 5 calls

    // To hold ASTs of functions not yet compiled
    std::map<std::string, std::unique_ptr<FunctionAST>> FunctionASTs;
    
    // To hold native function pointers of JIT'd functions
    std::map<std::string, void*> JITtedFunctions;


public:
    MinDynJIT();
    
    // Add a function's AST to our repository of known functions
    void addFunction(std::unique_ptr<FunctionAST> FuncAST);

    // Main entry point for execution of a function
    double execute(const std::string& FuncName);

    // The interpreter is a public member for fallback
    // For simplicity, we will define a basic interpreter logic inside JIT.cpp
    double interpret(const std::string& FuncName);
    double interpretExpr(ExprAST* Expr, std::map<std::string, double>& context);
};
