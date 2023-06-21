cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 20)

project(anggur)

file(GLOB anggur_source
	"source/Anggur/*/*/*.cpp"
	"source/Anggur/*/*.cpp"
	"source/Anggur/*.cpp"
)

add_library(anggur STATIC
	${anggur_source}
)

add_subdirectory("dependency/glfw")
add_subdirectory("dependency/glad")
add_subdirectory("dependency/stbtruetype")
add_subdirectory("dependency/stbimage")
add_subdirectory("dependency/uv")

target_include_directories(anggur PUBLIC "source")
target_link_libraries(anggur
	glfw
	glad
	stbtruetype
	stbimage
	uv

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
