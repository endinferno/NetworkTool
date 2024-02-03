cmake_minimum_required(VERSION 3.5)

set(BINARY_NAME StockCrawlerTest)
project(${BINARY_NAME})

enable_testing()

include_directories(${INCLUDE_LIST})
add_executable(${BINARY_NAME}
    ${SRC_LIST}
    ${POLLER_SRC}
    ${SOCKET_SRC_LIST}
    ${CONNECTOR_SRC_LIST}
    ${CLIENT_SRC_LIST}
    ${UTILS_SRC_LIST}
    ConnectorTest.cpp
    SinaStockParserTest.cpp
    IPAddressTest.cpp)

target_link_libraries(${BINARY_NAME} pthread)
target_link_libraries(${BINARY_NAME} fmt)
target_link_libraries(${BINARY_NAME} gtest gtest_main)
if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    target_link_libraries(${BINARY_NAME} iconv)
endif()
target_link_libraries(${BINARY_NAME} ssl)
target_link_libraries(${BINARY_NAME} crypto)

add_test(StockCrawlerTests ${BINARY_NAME})
