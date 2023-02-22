cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 20)

project(Anggur)

file(GLOB Anggur_Sources
	"Sources/Anggur/*/*.cpp"
	"Sources/Anggur/*.cpp"
)

add_library(Anggur STATIC
	${Anggur_Sources}
)

# Local libraries
add_subdirectory("Dependencies/GLAD/GLAD")
add_subdirectory("Dependencies/GLFW/GLFW")
add_subdirectory("Dependencies/STB/Image")
add_subdirectory("Dependencies/STB/TrueType")
add_subdirectory("Dependencies/SkyPjack/entt")

target_include_directories(Anggur PUBLIC "Sources")
target_link_libraries(Anggur
	glfw
	GLAD

	STBImage
	STBTrueType

    EnTT

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
