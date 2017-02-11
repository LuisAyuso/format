cmake_minimum_required (VERSION 2.6)
project (format)
add_executable(format src/main.cpp)

target_compile_options(format PRIVATE -std=c++14)
