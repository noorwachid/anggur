cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 20)

project(anggurcore)

file(GLOB anggurcore_source
	"src/Anggur/Core/*/*.cpp"
	"src/Anggur/Core/*.cpp"
)

add_library(anggurcore STATIC
	${anggurcore_source}
)

add_subdirectory("vendor/anggur/common")
add_subdirectory("vendor/anggur/math")
add_subdirectory("vendor/anggur/os")
add_subdirectory("vendor/anggur/graphics")

target_include_directories(anggurcore PUBLIC "src")
target_link_libraries(anggurcore
	anggurmath
	anggurcommon
	angguros
	anggurgraphics

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
