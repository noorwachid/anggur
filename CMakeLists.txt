cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 20)

project(Anggur)

file(GLOB anggur_src
    "src/Anggur/*/*.cpp"
	"src/Anggur/*.cpp"
)

add_library(anggur STATIC
    ${anggur_src}
)

# Local libraries
add_subdirectory(vendor/glad/glad)
add_subdirectory(vendor/glfw/glfw)
add_subdirectory(vendor/stb/image)
add_subdirectory(vendor/stb/truetype)
add_subdirectory(vendor/skypjack/entt)

target_include_directories(anggur PUBLIC src)
target_link_libraries(anggur
	glfw
	glad

	stb_image
	stb_truetype

    EnTT

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)
