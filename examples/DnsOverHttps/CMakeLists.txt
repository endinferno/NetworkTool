cmake_minimum_required(VERSION 3.5)

add_executable(DnsOverHttps DnsOverHttps.cpp)
target_link_libraries(DnsOverHttps NetworkToolLib)
