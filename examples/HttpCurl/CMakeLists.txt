cmake_minimum_required(VERSION 3.5)

add_executable(HttpCurl HttpCurl.cpp)
target_link_libraries(HttpCurl NetworkToolLib)
