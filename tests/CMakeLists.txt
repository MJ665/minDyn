# Enable testing for the project
enable_testing()

# Add a new executable for our test runner
add_executable(run_tests
    test_runner.cpp
    test_parser.cpp
)

# Link the test runner against the same LLVM libraries
target_link_libraries(run_tests PRIVATE ${LLVM_LIBS})

# Add the source directory to the include path so tests can find "Lexer.h" etc.
target_include_directories(run_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Add our compiled object files from the main executable
# This allows tests to call functions from Lexer.cpp, Parser.cpp etc.
target_link_libraries(run_tests PRIVATE $<TARGET_OBJECTS:mindyn_objects>)

# Define a test case using CTest
add_test(NAME UnitTests COMMAND run_tests)
