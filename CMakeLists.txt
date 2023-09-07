cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(anggur)

add_executable(anggur
	"source/Anggur/Text.cpp"
	"source/Anggur/Log.cpp"
	"source/Anggur/Application.cpp"

	"source/Anggur/OS/Window.cpp"

	"source/Anggur/Audio/AudioDevice.cpp"
	"source/Anggur/Audio/AudioBuffer.cpp"
	"source/Anggur/Audio/AudioSource.cpp"

	"source/Anggur/Graphics/Pointer.cpp"

	"source/Anggur/Graphics/VertexBuffer.cpp"
	"source/Anggur/Graphics/Shader.cpp"
	"source/Anggur/Graphics/Texture.cpp"
	"source/Anggur/Graphics/Image.cpp"
	"source/Anggur/Graphics/Font.cpp"
	"source/Anggur/Graphics/Font/FontPacker.cpp"

	"source/Anggur/Graphics/Renderer.cpp"
	"source/Anggur/Graphics/Renderer/CircleRenderer.cpp"
	"source/Anggur/Graphics/Renderer/LineRenderer.cpp"
	"source/Anggur/Graphics/Renderer/RectangleRenderer.cpp"
	"source/Anggur/Graphics/Renderer/TextRenderer.cpp"
	"source/Anggur/Graphics/Renderer/TriangleRenderer.cpp"

	"source/Anggur/Math.cpp"
	"source/Anggur/Math/Matrix3.cpp"
	"source/Anggur/Math/Matrix4.cpp"
	"source/Anggur/Math/Vector2.cpp"
	"source/Anggur/Math/Vector3.cpp"
	"source/Anggur/Math/Vector4.cpp"
	"source/Anggur/Math/Quaternion.cpp"
	"source/Anggur/Math/Random.cpp"

	"source/main.cpp"
)

add_subdirectory("dependency/glfw")
add_subdirectory("dependency/glad")
add_subdirectory("dependency/stbtruetype")
add_subdirectory("dependency/stbimage")
add_subdirectory("dependency/openalsoft")

target_include_directories(anggur PUBLIC "source")
target_link_libraries(anggur
	glfw
	glad
	stbtruetype
	stbimage

	OpenAL

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)

