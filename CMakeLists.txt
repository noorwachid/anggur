cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 20)

project(june)

file(GLOB anggur_source
	"src/Anggur/*/*/*.cpp"
	"src/Anggur/*/*.cpp"
	"src/Anggur/*.cpp"
)

add_library(anggur STATIC
	${anggur_source}
)

# Local libraries
add_subdirectory("vendor/glad/glad")
add_subdirectory("vendor/glfw/glfw")
add_subdirectory("vendor/stb/image")
add_subdirectory("vendor/stb/truetype")

add_subdirectory("vendor/anggur/math")
add_subdirectory("vendor/anggur/utility")

target_include_directories(anggur PUBLIC "src")
target_link_libraries(anggur
	glfw
	glad

	anggurmath
	anggurutility

	stbimage
	stbtruetype

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
