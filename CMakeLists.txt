cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(Anggur)

file(GLOB Anggur_Math

        "Source/Anggur/Math/*.cpp"
        "Source/Anggur/Math/*.hpp"
)

file(GLOB Anggur_Helper   

	"Source/Anggur/Helper/*.cpp"
	"Source/Anggur/Helper/*.hpp"
)

file(GLOB Anggur_Core

        "Source/Anggur/Core/*.cpp"
        "Source/Anggur/Core/*.hpp"
)

file(GLOB Anggur_CoreEvent

        "Source/Anggur/Core/Event/*.cpp"
        "Source/Anggur/Core/Event/*.hpp"
)

file(GLOB Anggur_Renderer

        "Source/Anggur/Renderer/*.cpp"
        "Source/Anggur/Renderer/*.hpp"
)

add_library(Anggur STATIC 

        ${Anggur_Math}
        ${Anggur_Helper}
        ${Anggur_Core}
        ${Anggur_CoreEvent}
        ${Anggur_Renderer}
)

# Local libraries
add_subdirectory(Vendor/Glad)
add_subdirectory(Vendor/Stb)

# SDL2
#add_subdirectory(Vendor/SDL2)
find_package(SDL2)

# SDL2_mixer
#add_subdirectory(Vendor/SDL2_mixer)
find_package(SDL2_mixer)

target_include_directories(Anggur PUBLIC Source)
target_include_directories(Anggur PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Anggur

	Glad
	Stb
	SDL2
        SDL2_mixer

	pthread
        ${CMAKE_DL_LIBS}
)
