cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 20)

project(Anggur)

file(GLOB anggur_source
    "source/Anggur/*/*.cpp"
	"source/Anggur/*.cpp"
)

add_library(anggur STATIC
	${anggur_source}
)

# Local libraries
add_subdirectory(dependency/davidde/glad)
add_subdirectory(dependency/glfw/glfw)
add_subdirectory(dependency/nothings/stbimage)
add_subdirectory(dependency/nothings/stbtruetype)

target_include_directories(anggur PUBLIC source)
target_link_libraries(anggur

	glfw
	glad

	stbimage
	stbtruetype

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
