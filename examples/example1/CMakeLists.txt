cmake_minimum_required(VERSION 3.9)

enable_language(CXX)

project(example1 VERSION "0.0.0")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

option(BUILD_SHARED_LIBS "Build shared libs" ON)
option(ENABLE_TESTING "Build and enable unit testing" ON)

find_package(panacea REQUIRED)

add_executable(example1 src/example1.cpp)
target_link_libraries(example1 panacea::panacea)
