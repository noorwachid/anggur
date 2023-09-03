cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)

project(anggur)

add_executable(anggur
	"source/Anggur/Text.cpp"
	"source/Anggur/Log.cpp"
	"source/Anggur/Application.cpp"

	"source/Anggur/OS/Window.cpp"

	"source/Anggur/Graphics/Pointer.cpp"

	"source/Anggur/Graphics/VertexBuffer.cpp"
	"source/Anggur/Graphics/Shader.cpp"
	"source/Anggur/Graphics/Texture.cpp"
	"source/Anggur/Graphics/Image.cpp"
	"source/Anggur/Graphics/Font.cpp"
	"source/Anggur/Graphics/Font/Packer.cpp"

	"source/Anggur/Graphics/Renderer.cpp"
	"source/Anggur/Graphics/Renderer/Circle.cpp"
	"source/Anggur/Graphics/Renderer/Line.cpp"
	"source/Anggur/Graphics/Renderer/Rectangle.cpp"
	"source/Anggur/Graphics/Renderer/Text.cpp"
	"source/Anggur/Graphics/Renderer/Triangle.cpp"

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
add_subdirectory("dependency/snd")

target_include_directories(anggur PUBLIC "source")
target_link_libraries(anggur
	glfw
	glad
	stbtruetype
	stbimage

	OpenAL
	sndfile

    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)

