cmake_minimum_required(VERSION 3.15)
project(relay)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use modern Boost config from Conan
find_package(Boost REQUIRED CONFIG COMPONENTS system)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(relay ${SOURCES})
target_link_libraries(relay Boost::boost Boost::system)
