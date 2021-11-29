cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(Anggur)

file(GLOB Anggur_SourceFiles

    "src/Anggur/Math/*.cpp"
    "src/Anggur/Helper/*.cpp"
    "src/Anggur/Core/*.cpp"
    "src/Anggur/Core/Event/*.cpp"
    "src/Anggur/Renderer/*.cpp"
    "src/Anggur/DS/*.cpp"
)

add_library(Anggur STATIC 

	${Anggur_SourceFiles}
)

# Local libraries
add_subdirectory(vendor/glad)
add_subdirectory(vendor/stb)

# SDL2
find_package(SDL2)
find_package(Threads)

target_include_directories(Anggur PUBLIC src PRIVATE ${SDL2_INCLUDE_DIRS})
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
