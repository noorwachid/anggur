cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(anggur)

add_library(anggur SHARED
	"source/Anggur/TextEncoding.cpp"
	"source/Anggur/Log.cpp"
	"source/Anggur/Application.cpp"

	"source/Anggur/OS/Window.cpp"
	"source/Anggur/OS/Input.cpp"

	"source/Anggur/Audio/AudioDevice.cpp"
	"source/Anggur/Audio/AudioBuffer.cpp"
	"source/Anggur/Audio/AudioSource.cpp"

	"source/Anggur/Graphics/Driver.cpp"

	"source/Anggur/Graphics/VertexBuffer.cpp"
	"source/Anggur/Graphics/Shader.cpp"
	"source/Anggur/Graphics/Texture.cpp"
	"source/Anggur/Graphics/Image.cpp"
	"source/Anggur/Graphics/Font.cpp"
	"source/Anggur/Graphics/FontPacker.cpp"

	"source/Anggur/Graphics/Renderer.cpp"
	"source/Anggur/Graphics/Renderers/CircleRenderer.cpp"
	"source/Anggur/Graphics/Renderers/LineRenderer.cpp"
	"source/Anggur/Graphics/Renderers/RectangleRenderer.cpp"
	"source/Anggur/Graphics/Renderers/TextRenderer.cpp"
	"source/Anggur/Graphics/Renderers/TriangleRenderer.cpp"

	"source/Anggur/Math.cpp"
	"source/Anggur/Math/Matrix3.cpp"
	"source/Anggur/Math/Matrix4.cpp"
	"source/Anggur/Math/Vector2.cpp"
	"source/Anggur/Math/Vector3.cpp"
	"source/Anggur/Math/Vector4.cpp"
	"source/Anggur/Math/Quaternion.cpp"
	"source/Anggur/Math/Random.cpp"
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

