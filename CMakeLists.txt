cmake_minimum_required(VERSION 3.5)

set(BINARY_NAME StockCrawler)
project(${BINARY_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(NOT POLL_TYPE)
    set(POLL_TYPE Epoll)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -Werror -DNDEBUG")
endif()

if(POLL_TYPE MATCHES Epoll)
    file(GLOB POLLER_SRC
        src/Poller/Epoller.cpp
        src/Poller/EventPoller.cpp)
    add_compile_definitions(POLL_TYPE=0)
elseif(POLL_TYPE MATCHES Poll)
    file(GLOB POLLER_SRC
        src/Poller/Poller.cpp
        src/Poller/EventPoller.cpp)
    add_compile_definitions(POLL_TYPE=1)
elseif(POLL_TYPE MATCHES Select)
    file(GLOB POLLER_SRC
        src/Poller/Selecter.cpp
        src/Poller/EventPoller.cpp)
    add_compile_definitions(POLL_TYPE=2)
else()
    file(GLOB POLLER_SRC
        src/Poller/Epoller.cpp
        src/Poller/EventPoller.cpp)
    add_compile_definitions(POLL_TYPE=0)
endif()

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

set(INCLUDE_LIST ${PROJECT_SOURCE_DIR}/include)

include_directories(${INCLUDE_LIST})

add_subdirectory(dependencies)

add_executable(${BINARY_NAME} Main.cpp ${SRC_LIST} ${POLLER_SRC})

target_link_libraries(${BINARY_NAME} pthread)
target_link_libraries(${BINARY_NAME} fmt)
