cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(Anggur)

file(GLOB Anggur_SourceFiles

    "Source/Anggur/Math/*.cpp"
    "Source/Anggur/Helper/*.cpp"
    "Source/Anggur/Core/*.cpp"
    "Source/Anggur/Core/Event/*.cpp"
    "Source/Anggur/Renderer/*.cpp"
)

add_library(Anggur STATIC 

	${Anggur_SourceFiles}
)

# Local libraries
add_subdirectory(Dependencies/Glad)
add_subdirectory(Dependencies/Stb)

# SDL2
find_package(SDL2)
find_package(SDL2_mixer)
find_package(Threads)

target_include_directories(Anggur PUBLIC Source PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Anggur

    Glad
    Stb
    SDL2

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)


# Installation 
# install(TARGETS Anggur DESTINATION /usr/lib)
# install(DIRECTORY Source/Anggur 
# 	DESTINATION /usr/include 
# 	FILES_MATCHING PATTERN "*.hpp")
