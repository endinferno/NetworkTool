cmake_minimum_required(VERSION 3.5)


add_executable(PingPongServer Server.cpp)
target_link_libraries(PingPongServer NetworkToolLib)

add_executable(PingPongClient Client.cpp)
target_link_libraries(PingPongClient NetworkToolLib)
