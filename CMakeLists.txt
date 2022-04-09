cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(anggur)

file(GLOB anggur_sources
    "src/Anggur/Math/*.cpp"
    "src/Anggur/Utility/*.cpp"
    "src/Anggur/Core/*.cpp"
    "src/Anggur/Core/Event/*.cpp"
    "src/Anggur/Graphic/*.cpp"
)

add_library(anggur STATIC
	${anggur_sources}
		example/MainScene.h src/Anggur/Math/Vector3.cpp src/Anggur/Math/Vector3.h src/Anggur/Core/Internal.h src/Anggur/Core/GraphicFunction.h src/Anggur/Core/Scene/Scene.cpp src/Anggur/Core/Scene/SceneManager.cpp src/Anggur/Core/Scene/SceneManager.h src/Anggur/Core/GraphicFunction.cpp)

# Local libraries
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/stb)

target_include_directories(anggur PUBLIC src PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(anggur

    glad
	glfw
    stb

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)

add_executable(anggur_example example/main.cpp)

target_link_libraries(anggur_example anggur)