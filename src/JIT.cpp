#include "JIT.h"
#include "CodeGen.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Support/TargetSelect.h"

MinDynJIT::MinDynJIT() {
    // These must be called before using the JIT
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    auto JITBuilder = llvm::orc::LLJITBuilder();
    auto TempJIT = JITBuilder.create();
    if (!TempJIT) {
        fprintf(stderr, "Failed to create LLJIT instance.\n");
        exit(1);
    }
    TheJIT = std::move(*TempJIT);
}

void MinDynJIT::addFunction(std::unique_ptr<FunctionAST> FuncAST) {
    FunctionASTs[FuncAST->Proto->getName()] = std::move(FuncAST);
}

double MinDynJIT::execute(const std::string& FuncName) {
    // Increment call count for profiling
    FunctionCallCounts[FuncName]++;
    
    // Check if function is already JIT-compiled
    if (JITtedFunctions.count(FuncName)) {
        void* FPtr = JITtedFunctions[FuncName];
        double (*FP)() = (double (*)())(intptr_t)FPtr;
        
        fprintf(stderr, "Executing JIT'd function '%s'\n", FuncName.c_str());
        return FP();
    }
    
    // If we've hit the threshold, JIT compile it
    if (FunctionCallCounts[FuncName] >= JIT_THRESHOLD) {
        fprintf(stderr, "JIT threshold met for '%s'. Compiling to native code...\n", FuncName.c_str());
        
        // Step 1: Initialize CodeGen globals
        TheContext = std::make_unique<llvm::LLVMContext>();
        TheModule = std::make_unique<llvm::Module>("MinDyn JIT", *TheContext);
        Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
        
        // Step 2: Generate LLVM IR from the function's AST
        auto It = FunctionASTs.find(FuncName);
        if (It == FunctionASTs.end()) {
            fprintf(stderr, "Error: Unknown function '%s' called.\n", FuncName.c_str());
            return 0;
        }
        It->second->codegen();
        
        // TheModule->print(llvm::errs(), nullptr); // DEBUG: Dump the LLVM IR

        // Step 3: Add the module to the JIT
        auto Err = TheJIT->addIRModule(llvm::orc::ThreadSafeModule(std::move(TheModule), std::move(TheContext)));
        if (Err) {
            fprintf(stderr, "Error adding module to JIT\n");
            return 0;
        }

        // Step 4: Look up the address of the compiled function
        auto Symbol = TheJIT->lookup(FuncName);
        if (!Symbol) {
            fprintf(stderr, "Error looking up JIT'd symbol\n");
            return 0;
        }

        // Step 5: Cast to a function pointer and call it
        void* FPtr = (void*)(intptr_t)Symbol->getAddress();
        JITtedFunctions[FuncName] = FPtr; // Cache the pointer
        
        double (*FP)() = (double (*)())(intptr_t)FPtr;
        return FP();
    }
    
    // If below threshold, use the interpreter
    fprintf(stderr, "Executing '%s' via interpreter (call #%d)\n", FuncName.c_str(), FunctionCallCounts[FuncName]);
    return interpret(FuncName);
}


// --- Simple Tree-Walking Interpreter ---

double MinDynJIT::interpret(const std::string& FuncName) {
    auto It = FunctionASTs.find(FuncName);
    if (It == FunctionASTs.end()) {
        fprintf(stderr, "Interpreter Error: Unknown function '%s'\n", FuncName.c_str());
        return 0;
    }
    std::map<std::string, double> context; // Empty context for top-level call
    return interpretExpr(It->second->Body.get(), context);
}

double MinDynJIT::interpretExpr(ExprAST* Expr, std::map<std::string, double>& context) {
    // This is a simplified interpreter that doesn't properly handle function calls with args yet.
    // It serves to demonstrate the fallback mechanism.
    if (auto Num = dynamic_cast<NumberExprAST*>(Expr)) {
        return Num->Val;
    }
    if (auto Bin = dynamic_cast<BinaryExprAST*>(Expr)) {
        double L = interpretExpr(Bin->LHS.get(), context);
        double R = interpretExpr(Bin->RHS.get(), context);
        switch (Bin->Op) {
            case '+': return L + R;
            case '-': return L - R;
            case '*': return L * R;
            case '/': return L / R;
            case '<': return L < R;
            case '>': return L > R;
        }
    }
    // A full interpreter would handle VariableExprAST, CallExprAST, IfExprAST etc.
    // This is just a placeholder to show the concept.
    fprintf(stderr, "Interpreter Error: Unsupported expression type.\n");
    return 0;
}
