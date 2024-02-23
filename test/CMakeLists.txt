cmake_minimum_required(VERSION 3.5)

set(TEST_BINARY_NAME "${PROJECT_NAME}Test")
enable_testing()

aux_source_directory(${PROJECT_SOURCE_DIR}/test TEST_SRC_LIST)
add_executable(${TEST_BINARY_NAME} ${TEST_SRC_LIST})
target_link_libraries(${TEST_BINARY_NAME} NetworkToolLib)
target_link_libraries(${TEST_BINARY_NAME} gtest gtest_main)
