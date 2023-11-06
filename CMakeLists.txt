cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(anggur)

add_library(anggur SHARED
	"source/anggur/textencoding.cpp"
	"source/anggur/log.cpp"
	"source/anggur/application.cpp"

	"source/anggur/os/window.cpp"
	"source/anggur/os/input.cpp"

	"source/anggur/audio/audiodevice.cpp"
	"source/anggur/audio/audiobuffer.cpp"
	"source/anggur/audio/audiosource.cpp"

	"source/anggur/graphics/driver.cpp"

	"source/anggur/graphics/vertexbuffer.cpp"
	"source/anggur/graphics/shader.cpp"
	"source/anggur/graphics/texture.cpp"
	"source/anggur/graphics/image.cpp"
	"source/anggur/graphics/font.cpp"
	"source/anggur/graphics/font/fontpacker.cpp"

	"source/anggur/graphics/renderer.cpp"
	"source/anggur/graphics/renderer/circlerenderer.cpp"
	"source/anggur/graphics/renderer/linerenderer.cpp"
	"source/anggur/graphics/renderer/rectanglerenderer.cpp"
	"source/anggur/graphics/renderer/textrenderer.cpp"
	"source/anggur/graphics/renderer/trianglerenderer.cpp"

	"source/anggur/math.cpp"
	"source/anggur/math/matrix3.cpp"
	"source/anggur/math/matrix4.cpp"
	"source/anggur/math/vector2.cpp"
	"source/anggur/math/vector3.cpp"
	"source/anggur/math/vector4.cpp"
	"source/anggur/math/quaternion.cpp"
	"source/anggur/math/random.cpp"
)

add_subdirectory("dependency/glfw")
add_subdirectory("dependency/glad")
add_subdirectory("dependency/stbtruetype")
add_subdirectory("dependency/stbimage")
add_subdirectory("dependency/stbvorbis")
add_subdirectory("dependency/openalsoft")

target_include_directories(anggur PUBLIC "source")
target_link_libraries(anggur
	glfw
	glad
	stbtruetype
	stbimage
	stbvorbis

	OpenAL

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)

