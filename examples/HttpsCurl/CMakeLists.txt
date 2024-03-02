cmake_minimum_required(VERSION 3.5)

add_executable(HttpsCurl HttpsCurl.cpp)
target_link_libraries(HttpsCurl NetworkToolLib)
