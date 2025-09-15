cmake_minimum_required(VERSION 3.16)
project(MinDynCompiler CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing for the project as a whole
enable_testing()

# Find LLVM
find_package(LLVM 14 REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# --- ADVANCED FIX ---
# Use file(GLOB ...) to programmatically find all .cpp files in the src directory.
# This can be more robust for paths with special characters or spaces.
file(GLOB COMPILER_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Create the object library from the list of sources we just found.
add_library(mindyn_objects OBJECT ${COMPILER_SOURCES})

# Create the main executable.
add_executable(mindyn
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    $<TARGET_OBJECTS:mindyn_objects>
)

# Link against LLVM libraries
llvm_map_components_to_libnames(LLVM_LIBS
    core
    orcjit
    executionengine
    support
    ir
    passes
    transformutils
)
target_link_libraries(mindyn PRIVATE ${LLVM_LIBS})

# Add the tests subdirectory.
add_subdirectory(tests)

# Add an install rule
install(TARGETS mindyn DESTINATION bin)

# Print LLVM info for debugging
message(STATUS "LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM_LIBRARY_DIRS: ${LLVM_LIBRARY_DIRS}")
message(STATUS "LLVM_LIBS: ${LLVM_LIBS}")