cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(anggur_example)

add_executable(anggur_example src/main.cpp)

target_link_libraries(anggur_example anggur)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})