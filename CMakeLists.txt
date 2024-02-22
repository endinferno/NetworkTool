cmake_minimum_required(VERSION 3.5)

set(BINARY_NAME StockCrawler)
project(${BINARY_NAME})

set(SERVER_BINARY_NAME "${BINARY_NAME}Server")
set(CLIENT_BINARY_NAME "${BINARY_NAME}Client")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (NOT POLL_TYPE)
    set(POLL_TYPE Epoll)
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -Werror -DNDEBUG")
endif()

if (POLL_TYPE MATCHES Epoll)
    file(GLOB POLLER_SRC
        src/Poller/Epoller.cpp
        src/Poller/EventPoller.cpp)
    add_compile_definitions(USE_EPOLL)
elseif(POLL_TYPE MATCHES Poll)
    file(GLOB POLLER_SRC
        src/Poller/Poller.cpp
        src/Poller/EventPoller.cpp)
    add_compile_definitions(USE_POLL)
elseif(POLL_TYPE MATCHES Select)
    file(GLOB POLLER_SRC
        src/Poller/Selector.cpp
        src/Poller/EventPoller.cpp)
    add_compile_definitions(USE_SELECT)
else()
    file(GLOB POLLER_SRC
        src/Poller/Epoller.cpp
        src/Poller/EventPoller.cpp)
    add_compile_definitions(USE_EPOLL)
endif()

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Socket SOCKET_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Connector CONNECTOR_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Client CLIENT_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Utils UTILS_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Dns DNS_SRC_LIST)
set(SOURCE_LIST
    ${SRC_LIST}
    ${POLLER_SRC}
    ${SOCKET_SRC_LIST}
    ${CONNECTOR_SRC_LIST}
    ${CLIENT_SRC_LIST}
    ${UTILS_SRC_LIST}
    ${DNS_SRC_LIST})

set(INCLUDE_LIST ${PROJECT_SOURCE_DIR}/include)

include_directories(${INCLUDE_LIST})

add_subdirectory(dependencies)

macro(EXECUTABLE BINARY_NAME MAIN_FILE)
    add_executable(${BINARY_NAME} ${MAIN_FILE} ${SOURCE_LIST})

    target_link_libraries(${BINARY_NAME} pthread)
    target_link_libraries(${BINARY_NAME} fmt)
    if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
        target_link_libraries(${BINARY_NAME} iconv)
    endif()
    target_link_libraries(${BINARY_NAME} ssl)
    target_link_libraries(${BINARY_NAME} crypto)
endmacro()

EXECUTABLE(${SERVER_BINARY_NAME} Server.cpp)
EXECUTABLE(${CLIENT_BINARY_NAME} Client.cpp)

add_subdirectory(test)
