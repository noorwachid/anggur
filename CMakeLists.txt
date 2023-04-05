cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 20)

project(Anggur)

file(GLOB Anggur_Source
	"Source/Anggur/*/*/*.cpp"
	"Source/Anggur/*/*.cpp"
	"Source/Anggur/*.cpp"
)

add_library(Anggur STATIC
	${Anggur_Source}
)

# Local libraries
add_subdirectory("Vendor/GLAD/GLAD")
add_subdirectory("Vendor/GLFW/GLFW")
add_subdirectory("Vendor/STB/Image")
add_subdirectory("Vendor/STB/TrueType")
add_subdirectory("Vendor/Chlumsky/MSDFGen")

target_include_directories(Anggur PUBLIC "Source")
target_link_libraries(Anggur
	glfw
	GLAD

	STBImage
	STBTrueType

	msdfgen::msdfgen

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
