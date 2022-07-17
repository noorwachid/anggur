cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 20)

project(anggur)

file(GLOB anggur_sources
    "src/Anggur/*/*.cpp"
    "src/Anggur/*.cpp"
)

add_library(anggur STATIC
	${anggur_sources}
)

# Local libraries
add_subdirectory(vendor/davidde/glad)
add_subdirectory(vendor/glfw/glfw)
add_subdirectory(vendor/nothings/stbimage)

target_include_directories(anggur PUBLIC src)
target_link_libraries(anggur

	glfw
	glad
	stb_image

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
