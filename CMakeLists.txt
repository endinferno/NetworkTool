cmake_minimum_required(VERSION 3.5)

set(BINARY_NAME StockCrawler)
project(${BINARY_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(NOT DEFINED POLL_TYPE)
    set(POLL_TYPE EPOLL)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -D${POLL_TYPE}")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -Werror -DNDEBUG -D${POLL_TYPE}")
endif()

if(POLL_TYPE MATCHES EPOLL)
    file(GLOB POLLER_SRC src/Poller/Epoller.cpp)
elseif(POLL_TYPE MATCHES POLL)
    file(GLOB POLLER_SRC src/Poller/Poller.cpp)
elseif(POLL_TYPE MATCHES SELECT)
    file(GLOB POLLER_SRC src/Poller/Selecter.cpp)
else()
    file(GLOB POLLER_SRC src/Poller/Epoller.cpp)
endif()

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

set(INCLUDE_LIST ${PROJECT_SOURCE_DIR}/include)

include_directories(${INCLUDE_LIST})

add_subdirectory(dependencies)

add_executable(${BINARY_NAME} Main.cpp ${SRC_LIST} ${POLLER_SRC})

target_link_libraries(${BINARY_NAME} pthread)
target_link_libraries(${BINARY_NAME} fmt)
