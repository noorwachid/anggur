cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(example)

file(GLOB example_sources
    "src/*.cpp"
    "src/Scenes/*.cpp"
)

add_executable(example ${example_sources})
target_link_libraries(example anggur)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
