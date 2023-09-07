em++ \
	-std=c++17 \
	-Isource \
	-Idependency/stbimage/src \
	-Idependency/stbtruetype/src \
	-s FULL_ES3=1 \
	-s USE_GLFW=3 \
	-lopenal \
	-s EXCEPTION_CATCHING_ALLOWED=catch \
	--preload-file "resource/sample.wav" \
	-o "web/index.html" \
	--shell-file "web/template.html" \
	"source/Anggur/Text.cpp" \
	"source/Anggur/Log.cpp" \
	"source/Anggur/Application.cpp" \
	"source/Anggur/Audio/Device.cpp" \
	"source/Anggur/Audio/Buffer.cpp" \
	"source/Anggur/Audio/Source.cpp" \
	"source/Anggur/OS/Window.cpp" \
	"source/Anggur/Graphics/Pointer.cpp" \
	"source/Anggur/Graphics/VertexBuffer.cpp" \
	"source/Anggur/Graphics/Shader.cpp" \
	"source/Anggur/Graphics/Texture.cpp" \
	"source/Anggur/Graphics/Image.cpp" \
	"source/Anggur/Graphics/Font.cpp" \
	"source/Anggur/Graphics/Font/Packer.cpp" \
	"source/Anggur/Graphics/Renderer.cpp" \
	"source/Anggur/Graphics/Renderer/Circle.cpp" \
	"source/Anggur/Graphics/Renderer/Line.cpp" \
	"source/Anggur/Graphics/Renderer/Rectangle.cpp" \
	"source/Anggur/Graphics/Renderer/Text.cpp" \
	"source/Anggur/Graphics/Renderer/Triangle.cpp" \
	"source/Anggur/Math.cpp" \
	"source/Anggur/Math/Matrix3.cpp" \
	"source/Anggur/Math/Matrix4.cpp" \
	"source/Anggur/Math/Vector2.cpp" \
	"source/Anggur/Math/Vector3.cpp" \
	"source/Anggur/Math/Vector4.cpp" \
	"source/Anggur/Math/Quaternion.cpp" \
	"source/Anggur/Math/Random.cpp" \
	"source/main.cpp"

