cmake_minimum_required(VERSION 3.14)
project(DrivaPi_MotorSensor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable CTest
enable_testing()

# Add our source code library
add_subdirectory(src)

# Add our tests
add_subdirectory(tests)
