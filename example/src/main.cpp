#include <Anggur/Core/Application.h>
#include <Anggur/Graphic/Function.h>
#include <Anggur/Graphic/Shader.h>
#include <Anggur/Graphic/VertexArray.h>
#include <Anggur/Utility/Log.h>

using namespace Anggur;

class Rx {
public:
	static void initialize() {
		vertices = {
			// position             // normal            // color                   // texCoord     // texSlot
			 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, -1.0f,	 1.0f, 0.1f, 0.2f, 1.0f, 	0.0f, 1.0f, 	0.0f,
			-0.5f,  0.5f, 0.0f,		0.0f, 0.0f, -1.0f,	 1.0f, 0.0f, 0.9f, 1.0f, 	0.0f, 1.0f, 	0.0f,
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, -1.0f,	 0.0f, 0.3f, 1.0f, 1.0f, 	0.0f, 1.0f, 	0.0f,
			 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, -1.0f,	 0.0f, 1.0f, 0.3f, 1.0f, 	0.0f, 1.0f, 	0.0f,
		};

		indices = {
			0, 1, 2,
			2, 3, 0,
		};

		initializeVertexArray();
		initializeShader();
	}

	static void beginScene() {
	}

	static void endScene() {
	}

	static void update() {
		shader.bind();
		
		vertexBuffer.bind();
		vertexBuffer.setData(vertices);

		indexBuffer.bind();
		indexBuffer.setData(indices);

		vertexArray.bind();
		vertexArray.drawUntil(indices.size());
	}

private:
	static void initializeVertexArray() {

		vertexBuffer.create();
		vertexBuffer.bind();
		vertexBuffer.setCapacity(vertices.size());

		vertexArray.create();
		vertexArray.bind();
		vertexArray.setAttribute(0, 13, 3, 0);
		vertexArray.setAttribute(1, 13, 3, 3);
		vertexArray.setAttribute(2, 13, 4, 6);
		vertexArray.setAttribute(3, 13, 2, 10);
		vertexArray.setAttribute(4, 13, 1, 12);
		
		indexBuffer.create();
		indexBuffer.bind();
		indexBuffer.setCapacity(indices.size());
	}

	static void initializeShader() {
		shader.bind();
		shader.setVertexSource(R"(
			#version 330 core

			layout (location = 0) in vec3 aPosition;
			layout (location = 1) in vec3 aNormal;
			layout (location = 2) in vec4 aColor;
			layout (location = 3) in vec2 aTexCoord;
			layout (location = 4) in float aTexSlot;

			out vec3 vNormal;
			out vec4 vColor;
			out vec2 vTexCoord;
			out float vTexSlot;

			void main() {
			    gl_Position = vec4(aPosition, 1.0);

				vNormal = aNormal;
				vColor = aColor;
				vTexCoord = aTexCoord;
				vTexSlot = aTexSlot;
			}
		)");

		shader.setFragmentSource(R"(
			#version 330 core
			
			in vec3 vNormal;
			in vec4 vColor;
			in vec2 vTexCoord;
			in float vTexSlot;

			out vec4 fColor;
			
			void main() {
			   fColor = vColor;
			}
		)");

		shader.compile();
	}

private:
	static Shader shader;
	static VertexArray vertexArray;
	static VertexBuffer vertexBuffer;
	static IndexBuffer indexBuffer;
	static std::vector<float> vertices;
	static std::vector<uint32_t> indices;
};


Shader Rx::shader;
VertexArray Rx::vertexArray;
VertexBuffer Rx::vertexBuffer;
IndexBuffer Rx::indexBuffer;
std::vector<float> Rx::vertices;
std::vector<uint32_t> Rx::indices;

class Example3D: public Node {
public:
	void initialize() override {
		Rx::initialize();
	}

	void update() override {
		Rx::beginScene();

		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		Rx::update();

		Rx::endScene();
	}
};

int main() {
	Application::initialize(Vector2(400, 400), "Anggur Example");
	Application::run(new Example3D());
}