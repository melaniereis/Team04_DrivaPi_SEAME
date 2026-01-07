cmake_minimum_required(VERSION 3.14)
project(DrivaPi_Unit_Tests C)

# Enable CTest
enable_testing()

# Find Bash to run our script
find_program(BASH_EXECUTABLE bash REQUIRED)

# Register the Master Script as a test
# Note: ${CMAKE_CURRENT_SOURCE_DIR} is 'tests/'
add_test(NAME "Ceedling_Suite"
    COMMAND ${BASH_EXECUTABLE} unit/run_all_tests.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/unit
)

# Set properties (timeout, labels)
set_tests_properties("Ceedling_Suite" PROPERTIES
    LABELS "ceedling"
    TIMEOUT 600
)