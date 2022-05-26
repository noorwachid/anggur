cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 20)

project(anggur)

file(GLOB anggur_sources
    "src/Anggur/Window/*.cpp"
    "src/Anggur/Event/*.cpp"
    "src/Anggur/Scene/*.cpp"
    "src/Anggur/Graphics/*.cpp"
    "src/Anggur/Math/*.cpp"
    "src/Anggur/System/*.cpp"
    "src/Anggur/Utility/*.cpp"
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
