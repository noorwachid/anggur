cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(Application)

file(GLOB Application_sources
    "Sources/*.cpp"
    "Sources/*/*.cpp"
)

add_executable(Application ${Application_sources})
target_link_libraries(Application Anggur)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
