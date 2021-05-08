cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(Anggur)

file(GLOB Anggur_Helper   

	"Source/Anggur/Helper/*.cpp"
	"Source/Anggur/Helper/*.hpp"
)

file(GLOB Anggur_Math     

	"Source/Anggur/Math/*.cpp"
	"Source/Anggur/Math/*.hpp"
)

file(GLOB Anggur_Renderer 

	"Source/Anggur/Renderer/*.cpp"
	"Source/Anggur/Renderer/*.hpp"
)

file(GLOB Anggur_System   

	"Source/Anggur/System/*.cpp"
	"Source/Anggur/System/*.hpp"
)

file(GLOB Anggur_SystemEvent

        "Source/Anggur/System/Event/*.cpp"
        "Source/Anggur/System/Event/*.hpp"
)

add_library(Anggur STATIC 

	${Anggur_Helper}
	${Anggur_Math}
        ${Anggur_Renderer}
        ${Anggur_System}
        ${Anggur_SystemEvent}
)

# Local libraries
add_subdirectory(Vendor/Glad)
add_subdirectory(Vendor/Stb)

# SDL2
# Swap these two line to switch from system installed library to local library
# add_subdirectory(Vendor/SDL2)
find_package(SDL2)

# Posix thread
find_package(pthread)

target_include_directories(Anggur PUBLIC Source)
target_include_directories(Anggur PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Anggur

	Glad
	Stb
	SDL2

	pthread
        ${CMAKE_DL_LIBS}
)
