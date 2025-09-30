
# Min-Dyn: A Compiler Backend for a Dynamic Language

![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)
![Build](https://img.shields.io/badge/Build-CMake-green.svg)
![Framework](https://img.shields.io/badge/Framework-LLVM%2014-orange.svg)
![License](https://img.shields.io/badge/License-MIT-lightgrey.svg)

---

### Project Context

This repository was developed as a self-driven initiative to gain practical, hands-on experience with the advanced compiler backend concepts central to modern managed runtimes. It is directly inspired by and aligned with the goals of the **"Compiler Backend for Managed Runtimes"** project under **PI Manas Thakur**.

## Overview

**JoltCompiler** is a complete, educational compiler built from scratch for "Jolt," a minimal, dynamically-typed programming language. The project's primary goal is to demonstrate the architecture and core principles of a modern, performance-oriented language runtime.

It implements a **hybrid execution model**, a technique used in high-performance virtual machines (like the JVM and V8). Code begins execution in a simple, portable interpreter. A runtime profiler identifies performance-critical "hot spots," which are then compiled to highly optimized native machine code at runtime by a Just-In-Time (JIT) engine.

## Core Features

*   **Hand-written Lexer & Parser:** A recursive descent parser that builds an Abstract Syntax Tree (AST) from source code.
*   **Tree-Walking Interpreter:** A baseline execution engine that directly evaluates the AST. It's simple and serves as a fallback for code that is not performance-critical.
*   **LLVM-based JIT Engine:** The high-performance core. It uses the LLVM C++ API and its ORC (On-Request Compilation) JIT framework to translate the AST of hot functions into optimized machine code.
*   **Feedback-Driven Optimization (FDO):** A simple, counter-based profiler monitors function invocation counts. When a function exceeds a predefined threshold, it triggers JIT compilation.
*   **Modular & Testable:** Built with CMake and includes an integrated testing suite (`CTest`) for verifying component correctness.

## High-Level Architecture

The compiler follows a standard multi-stage pipeline, with a dynamic choice between two execution backends.

```
[ Jolt Source Code ] -> [ Lexer ] -> [ Parser ] -> [ Abstract Syntax Tree (AST) ]
                                                            |
                 +------------------------------------------+------------------------------------------+
                 |                                                                                      |
                 v                                                                                      v
      [ Interpreter Backend ]                                                               [ JIT Backend (LLVM) ]
 (Executes all code initially)                                                        (Compiles & executes "hot" code)
                 |                                                                                      ^
                 +------------------------------------>[ Profiler ]------------------------------------+
                                                (Identifies hot spots and triggers JIT)
```

## The Jolt Language

Jolt is a simple expression-based language designed to showcase the compiler's features.

**Example (`factorial.jolt`):**
```c
# Function definitions use the 'def' keyword.
def factorial(n)
    if n < 2 then
        1  # Expressions are implicitly returned.
    else
        n * factorial(n - 1);

# Top-level expressions are executed as the main entry point.
factorial(10);
```

## Getting Started: Build and Run

The project is configured to build on macOS and Linux. The easiest way to get started is with the provided build script, which handles all dependencies and configuration steps.

### Prerequisites

*   **On macOS:**
    *   Xcode Command Line Tools (`xcode-select --install`)
    *   Homebrew (`/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`)
    *   LLVM 14 via Homebrew: `brew install llvm@14`
*   **On Linux (Ubuntu/Debian):**
    *   `sudo apt-get update && sudo apt-get install build-essential cmake git llvm-14-dev`
*   **Using Docker (Platform Independent):**
    *   An installed Docker engine.

### Build & Run Instructions

The included `run_project.sh` script automates the entire process.

```bash
# 1. Make the script executable
chmod +x run_project.sh

# 2. Run the script
# This will clean, configure, build, run the example, and run tests.
./run_project.sh
```

### Manual Build

If you prefer to build manually:

```bash
# 1. Create and enter a build directory
mkdir build && cd build

# 2. Configure with CMake (Example for macOS)
# The run_project.sh script handles this logic automatically.
cmake -D CMAKE_PREFIX_PATH=$(brew --prefix llvm@14) ..

# 3. Compile
make -j$(nproc) # Use nproc on Linux, sysctl -n hw.ncpu on macOS
```

## Usage

Once built, the main executable `mindyn` (or `JoltCompiler`) will be in the `build/` directory.

```bash
# Run an example file using the JIT engine (recommended)
./build/mindyn --jit examples/factorial.jolt

# Run an example file using only the interpreter
./build/mindyn examples/factorial.jolt

# Run the unit test suite
cd build/
ctest --verbose
```

## Project Structure

```
.
├── CMakeLists.txt        # Main CMake build script
├── README.md             # This file
├── run_project.sh        # Automated build and run script
├── examples/             # Example Jolt source files
│   ├── factorial.jolt
│   └── ...
├── src/                  # Core compiler source code
│   ├── AST.h             # Abstract Syntax Tree class definitions
│   ├── CodeGen.h         # Global objects for LLVM IR generation
│   ├── JIT.h / .cpp      # The JIT Engine and Interpreter implementation
│   ├── Lexer.h / .cpp    # Lexical analyzer
│   ├── Parser.h / .cpp   # Parser and AST builder
│   └── main.cpp          # Main driver for the compiler
└── tests/                # Unit and integration tests
    ├── CMakeLists.txt    # Build script for tests
    └── test_parser.cpp   # Example tests for the parser
```

## 6-Month Intern Development Roadmap

This project serves as a strong foundation. The following is a potential roadmap for extending its capabilities over a 6-month internship.

*   **Month 1: Language & Interpreter Enhancement**
    *   **Week 1-2:** Solidify language basics. Implement full support for `if/then/else` statements, boolean types (`true`/`false`), and additional comparison operators (`==`, `!=`, `<=`, `>=`).
    *   **Week 3-4:** Enhance the interpreter to fully support function calls with arguments and variable scoping, making it a complete execution engine on its own.

*   **Month 2: Deep Dive into LLVM & Optimization**
    *   **Week 5-6:** Implement an LLVM pass to print the generated, unoptimized IR for every JIT-compiled function. This provides critical insight into the code generation process.
    *   **Week 7-8:** Integrate LLVM's standard optimization pipeline. Add a pass manager in `JIT.cpp` to run passes like Instruction Combining, Reassociation, and GVN on the generated IR. Compare performance before and after.

*   **Month 3: Advanced Profiling**
    *   **Week 9-12:** Upgrade the profiler from a simple function-call counter to a **basic block counter**. This involves instrumenting the generated LLVM IR to increment counters at the start of each block, providing a much more granular view of hot paths *within* a function.

*   **Month 4: Implementing True Feedback-Driven Optimization**
    *   **Week 13-16:** Use the basic block profile data to implement a significant FDO. A prime candidate is **function inlining**. Use the profile data to decide whether to inline a function call at a specific call site, recompiling the caller with the callee's body embedded.

*   **Month 5: Advanced Language Features & Memory**
    *   **Week 17-20:** Implement support for a heap-allocated data type, such as arrays or simple objects. This is the first step towards a managed runtime and introduces the need for memory management.

*   **Month 6: Garbage Collection & Final Presentation**
    *   **Week 21-24:** Research and implement a **simple, stop-the-world, mark-and-sweep garbage collector** to manage the heap-allocated objects created in Month 5. Document the entire project, prepare a final presentation on the architecture, the FDOs implemented, and the performance gains achieved.



## OUTPUT:

```bash
╰─ /bin/bash "/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/script.sh"                                                  ─╯
🚀 Starting the ROBUST build and run process for the compiler project...
🧹 [Step 1/5] Forcefully cleaning up previous build directory...
✅ Cleanup complete.

🍏 Detected macOS. Performing detailed LLVM setup...
🔍 Found LLVM installation at: /opt/homebrew/opt/llvm@14
✅ Set C/C++ compilers to the ones in the LLVM prefix.
✅ Verified 'LLVMConfig.cmake' location.

⚙️  [Step 2/5] Configuring project with CMake...
-- The CXX compiler identification is Clang 14.0.6
-- The C compiler identification is Clang 14.0.6
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /opt/homebrew/opt/llvm@14/bin/clang++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /opt/homebrew/opt/llvm@14/bin/clang - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Performing Test HAVE_FFI_CALL
-- Performing Test HAVE_FFI_CALL - Success
-- Found FFI: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib/libffi.tbd
-- Performing Test Terminfo_LINKABLE
-- Performing Test Terminfo_LINKABLE - Success
-- Found Terminfo: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib/libcurses.tbd
-- Found ZLIB: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib/libz.tbd (found version "1.2.12")
-- Found LibXml2: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib/libxml2.tbd (found version "2.9.13")
-- LLVM_INCLUDE_DIRS: /opt/homebrew/opt/llvm@14/include
-- LLVM_LIBRARY_DIRS: /opt/homebrew/opt/llvm@14/lib
-- LLVM_LIBS: LLVMCore;LLVMOrcJIT;LLVMExecutionEngine;LLVMSupport;LLVMir;LLVMPasses;LLVMTransformUtils
-- Configuring done (2.1s)
-- Generating done (0.0s)
-- Build files have been written to: /Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/build
✅ CMake configuration complete.

🛠️  [Step 3/5] Compiling the project...
[ 37%] Building CXX object CMakeFiles/mindyn_objects.dir/src/JIT.cpp.o
[ 37%] Building CXX object CMakeFiles/mindyn_objects.dir/src/main.cpp.o
[ 37%] Building CXX object CMakeFiles/mindyn_objects.dir/src/AST_CodeGen.cpp.o
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/main.cpp:1:10: fatal error: 'Lexer.h' file not found
#include "Lexer.h"
         ^~~~~~~~~
1 error generated.
make[2]: *** [CMakeFiles/mindyn_objects.dir/src/main.cpp.o] Error 1
make[2]: *** Waiting for unfinished jobs....
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:38:29: error: out-of-line definition of 'codegen' does not match any declaration in 'NumberExprAST'
llvm::Value *NumberExprAST::codegen() {
                            ^~~~~~~
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:42:31: error: out-of-line definition of 'codegen' does not match any declaration in 'VariableExprAST'
llvm::Value *VariableExprAST::codegen() {
                              ^~~~~~~
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:52:29: error: out-of-line definition of 'codegen' does not match any declaration in 'BinaryExprAST'
llvm::Value *BinaryExprAST::codegen() {
                            ^~~~~~~
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:53:25: error: no member named 'codegen' in 'ExprAST'
  llvm::Value *L = LHS->codegen();
                   ~~~~~^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:54:25: error: no member named 'codegen' in 'ExprAST'
  llvm::Value *R = RHS->codegen();
                   ~~~~~^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:85:27: error: out-of-line definition of 'codegen' does not match any declaration in 'CallExprAST'
llvm::Value *CallExprAST::codegen() {
                          ^~~~~~~
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:97:30: error: no member named 'codegen' in 'ExprAST'
    ArgsV.push_back(Args[i]->codegen());
                    ~~~~~~~~~^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:105:14: error: use of undeclared identifier 'IfExprAST'; did you mean 'ExprAST'?
llvm::Value *IfExprAST::codegen() {
             ^~~~~~~~~
             ExprAST
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST.h:9:7: note: 'ExprAST' declared here
class ExprAST {
      ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:105:25: error: out-of-line definition of 'codegen' does not match any declaration in 'ExprAST'
llvm::Value *IfExprAST::codegen() {
                        ^~~~~~~
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:106:26: error: use of undeclared identifier 'Cond'
    llvm::Value *CondV = Cond->codegen();
                         ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:125:26: error: use of undeclared identifier 'Then'
    llvm::Value *ThenV = Then->codegen();
                         ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:132:18: error: no member named 'insert' in 'llvm::Function'
    TheFunction->insert(TheFunction->end(), ElseBB);
    ~~~~~~~~~~~  ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:134:26: error: use of undeclared identifier 'Else'; did you mean 'else'?
    llvm::Value *ElseV = Else ? Else->codegen() : llvm::ConstantFP::get(*TheContext, llvm::APFloat(0.0)); // Default to 0 if no else
                         ^~~~
                         else
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:134:26: error: expected expression
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:141:18: error: no member named 'insert' in 'llvm::Function'
    TheFunction->insert(TheFunction->end(), MergeBB);
    ~~~~~~~~~~~  ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:151:31: error: out-of-line definition of 'codegen' does not match any declaration in 'PrototypeAST'
llvm::Function *PrototypeAST::codegen() {
                              ^~~~~~~
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST_CodeGen.cpp:169:30: error: out-of-line definition of 'codegen' does not match any declaration in 'FunctionAST'
llvm::Function *FunctionAST::codegen() {
                             ^~~~~~~
17 errors generated.
make[2]: *** [CMakeFiles/mindyn_objects.dir/src/AST_CodeGen.cpp.o] Error 1
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/JIT.cpp:24:27: error: 'Proto' is a private member of 'FunctionAST'
    FunctionASTs[FuncAST->Proto->getName()] = std::move(FuncAST);
                          ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST.h:60:33: note: implicitly declared private here
  std::unique_ptr<PrototypeAST> Proto;
                                ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/JIT.cpp:55:21: error: no member named 'codegen' in 'FunctionAST'
        It->second->codegen();
        ~~~~~~~~~~~~^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/JIT.cpp:96:38: error: 'Body' is a private member of 'FunctionAST'
    return interpretExpr(It->second->Body.get(), context);
                                     ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST.h:61:28: note: implicitly declared private here
  std::unique_ptr<ExprAST> Body;
                           ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/JIT.cpp:103:21: error: 'Val' is a private member of 'NumberExprAST'
        return Num->Val;
                    ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST.h:16:10: note: implicitly declared private here
  double Val;
         ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/JIT.cpp:106:39: error: 'LHS' is a private member of 'BinaryExprAST'
        double L = interpretExpr(Bin->LHS.get(), context);
                                      ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST.h:31:28: note: implicitly declared private here
  std::unique_ptr<ExprAST> LHS, RHS;
                           ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/JIT.cpp:107:39: error: 'RHS' is a private member of 'BinaryExprAST'
        double R = interpretExpr(Bin->RHS.get(), context);
                                      ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST.h:31:33: note: implicitly declared private here
  std::unique_ptr<ExprAST> LHS, RHS;
                                ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/JIT.cpp:108:22: error: 'Op' is a private member of 'BinaryExprAST'
        switch (Bin->Op) {
                     ^
/Users/meet/Downloads/CivilEngineeringIITB/IITBCSECompilerProject/Compiler Backend for Managed Runtimes/min-dyn-compiler/src/AST.h:30:8: note: implicitly declared private here
  char Op;
       ^
7 errors generated.
make[2]: *** [CMakeFiles/mindyn_objects.dir/src/JIT.cpp.o] Error 1
make[1]: *** [CMakeFiles/mindyn_objects.dir/all] Error 2
make: *** [all] Error 2

╭─ ~/Dow/C/I/Compiler Backend for Managed Runtimes/min-dyn-compiler  on master !1 ─────────────────────────────────────────────────────────────────────────────────── 2 х  took 5s  at 03:53:52 AM ─╮
╰─                                                                                                                                                                                                 ─╯
```