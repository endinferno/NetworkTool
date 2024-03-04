cmake_minimum_required(VERSION 3.5)

add_executable(DnsResolver DnsResolver.cpp)
target_link_libraries(DnsResolver NetworkToolLib)
