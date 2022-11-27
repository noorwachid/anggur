cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(Application)

file(GLOB application_source
    "source/*.cpp"
    "source/*/*.cpp"
)

add_executable(application ${application_source})
target_link_libraries(application anggur)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
