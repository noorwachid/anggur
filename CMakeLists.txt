cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 20)

project(anggurcore)

file(GLOB anggurcore_source
	"src/Anggur/*/*/*.cpp"
	"src/Anggur/*/*.cpp"
)

add_library(anggurcore STATIC
	${anggurcore_source}
)

add_subdirectory("vendor/glfw/glfw")
add_subdirectory("vendor/glad/glad")
add_subdirectory("vendor/stb/truetype")
add_subdirectory("vendor/stb/image")

target_include_directories(anggurcore PUBLIC "src")
target_link_libraries(anggurcore
	glfw
	glad
	stbtruetype
	stbimage

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
