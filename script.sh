#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

echo "🚀 Starting the ROBUST build and run process for the compiler project..."

# --- 1. Cleanup ---
# A clean start is critical to avoid CMake caching issues.
echo "🧹 [Step 1/5] Forcefully cleaning up previous build directory..."
if [ -d "build" ]; then
    rm -rf build
fi
mkdir build
echo "✅ Cleanup complete."
echo

# --- 2. Find LLVM and Set Compilers (macOS Specific) ---
# This section is the core of the fix.
if [[ "$(uname)" == "Darwin" ]]; then
    echo "🍏 Detected macOS. Performing detailed LLVM setup..."
    
    # First, ensure Homebrew is installed.
    if ! command -v brew &> /dev/null; then
        echo "❌ FATAL: Homebrew is not installed. Please install it to manage dependencies."
        exit 1
    fi

    # Find the installation path of llvm@14. Exit if not found.
    LLVM_PREFIX=$(brew --prefix llvm@14 2>/dev/null || true)
    if [[ -z "$LLVM_PREFIX" ]]; then
        echo "❌ FATAL: LLVM 14 not found via Homebrew. Please run 'brew install llvm@14'."
        exit 1
    fi
    echo "🔍 Found LLVM installation at: $LLVM_PREFIX"

    # CRITICAL FIX 1: Explicitly tell the system to use the Clang compiler
    # that was installed with this version of LLVM.
    export CC="$LLVM_PREFIX/bin/clang"
    export CXX="$LLVM_PREFIX/bin/clang++"
    echo "✅ Set C/C++ compilers to the ones in the LLVM prefix."

    # CRITICAL FIX 2: Define the EXACT directory for CMake's find_package.
    # This is more direct than CMAKE_PREFIX_PATH.
    LLVM_CMAKE_DIR="$LLVM_PREFIX/lib/cmake/llvm"

    # Sanity check: Make sure the directory and file actually exist.
    if [ ! -f "$LLVM_CMAKE_DIR/LLVMConfig.cmake" ]; then
        echo "❌ FATAL: Cannot find 'LLVMConfig.cmake' in the expected directory: $LLVM_CMAKE_DIR"
        echo "Your Homebrew LLVM installation might be corrupted. Try 'brew reinstall llvm@14'."
        exit 1
    fi
    echo "✅ Verified 'LLVMConfig.cmake' location."
    
    # Construct the CMake command with the direct path.
    CMAKE_CMD="cmake -D LLVM_DIR=$LLVM_CMAKE_DIR .."
else
    # For Linux and other systems
    CMAKE_CMD="cmake .."
fi
echo

# --- 3. Configure with CMake ---
echo "⚙️  [Step 2/5] Configuring project with CMake..."
cd build
$CMAKE_CMD
echo "✅ CMake configuration complete."
echo

# --- 4. Compile with Make ---
echo "🛠️  [Step 3/5] Compiling the project..."
if [[ "$(uname)" == "Darwin" ]]; then
    CPU_CORES=$(sysctl -n hw.ncpu)
else
    CPU_CORES=$(nproc)
fi
make -j$CPU_CORES
echo "✅ Compilation successful."
echo

# --- 5. Run the Compiler & Tests ---
echo "⚡️ [Step 4/5] Running compiler on 'factorial.mindyn' with JIT enabled..."
./mindyn --jit ../examples/factorial.mindyn
echo "✅ Example run complete."
echo

echo "🧪 [Step 5/5] Running unit tests..."
ctest --verbose
echo "✅ Testing complete."
echo

echo "🎉 All steps completed successfully! Project is ready."