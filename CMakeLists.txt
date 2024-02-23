cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME NetworkTool)
project(${PROJECT_NAME})

set(LIBRARY_NAME "${PROJECT_NAME}Lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -Werror -DNDEBUG")
endif()

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Socket SOCKET_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Poller POLLER_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Connector CONNECTOR_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Client CLIENT_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Acceptor ACCEPTOR_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Utils UTILS_SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Dns DNS_SRC_LIST)
set(SOURCE_LIST
    ${SRC_LIST}
    ${POLLER_SRC}
    ${SOCKET_SRC_LIST}
    ${CONNECTOR_SRC_LIST}
    ${CLIENT_SRC_LIST}
    ${ACCEPTOR_SRC_LIST}
    ${UTILS_SRC_LIST}
    ${DNS_SRC_LIST})

set(INCLUDE_LIST ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(dependencies)

macro(LINK_LIBRARY LIBRARY_NAME)
    target_link_libraries(${LIBRARY_NAME} pthread)
    target_link_libraries(${LIBRARY_NAME} fmt)
    if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
        target_link_libraries(${LIBRARY_NAME} iconv)
    endif()
    target_link_libraries(${LIBRARY_NAME} ssl)
    target_link_libraries(${LIBRARY_NAME} crypto)
endmacro()

add_library(${LIBRARY_NAME} STATIC ${SOURCE_LIST})
target_include_directories(${LIBRARY_NAME} PUBLIC ${INCLUDE_LIST})
LINK_LIBRARY(${LIBRARY_NAME})
install(TARGETS ${LIBRARY_NAME} DESTINATION lib)

add_subdirectory(examples)

add_subdirectory(test)
