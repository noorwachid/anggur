cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(anggur)

file(GLOB anggur_sources
    "src/Anggur/Math/*.cpp"
    "src/Anggur/Utility/*.cpp"
    "src/Anggur/Core/*.cpp"
    "src/Anggur/Core/Event/*.cpp"
    "src/Anggur/Core/Scene/*.cpp"
    "src/Anggur/Graphic/*.cpp"
)

add_library(anggur STATIC
	${anggur_sources}
)

# Local libraries
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/stb)

target_include_directories(anggur PUBLIC src PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(anggur

    glad
	glfw
    stb

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)

add_subdirectory(example)