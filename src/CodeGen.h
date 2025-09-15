#pragma once

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <map>
#include <string>
#include <memory>

// This file centralizes the core LLVM objects needed for code generation.
// They are declared here as 'extern' so that they can be accessed from
// different parts of the compiler. The actual objects will be defined in JIT.cpp.

// The LLVMContext is an opaque class that owns and manages core LLVM data structures.
extern std::unique_ptr<llvm::LLVMContext> TheContext;

// The Module is LLVM's container for all other IR objects (like functions).
extern std::unique_ptr<llvm::Module> TheModule;

// The IRBuilder is a helper class that makes it easy to generate LLVM instructions.
extern std::unique_ptr<llvm::IRBuilder<>> Builder;

// NamedValues keeps track of which values are in the current scope.
// This is our symbol table for the code generator.
extern std::map<std::string, llvm::AllocaInst *> NamedValues;
