#include "Rx.h"
#include <Anggur/Math/Matrix4.h>
#include <iostream>

std::unique_ptr<Shader> Rx::shader;
std::unique_ptr<VertexArray> Rx::vertexArray;
std::unique_ptr<VertexBuffer> Rx::vertexBuffer;
std::unique_ptr<IndexBuffer> Rx::indexBuffer;
std::vector<float> Rx::vertices;
std::vector<uint32_t> Rx::indices;

Matrix4 model;

void Rx::initialize() {
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

    // model = Matrix4::createRotationZ(Math::pi / 4) * Matrix4::createTranslation(Vector3(0.5f, 0.5f, 0.0f));
    model = Matrix4::createTranslation(Vector3(0.5f, 0.5f, 0.0f)) * Matrix4::createRotationZ(Math::pi / 4);

    std::cout << model.toString();
}

void Rx::beginScene() {
}

void Rx::endScene() {
}

void Rx::update() {
    shader->bind();
    shader->setUniformMatrix4("uModel", model);
    
    vertexBuffer->bind();
    vertexBuffer->setData(vertices);

    indexBuffer->bind();
    indexBuffer->setData(indices);

    vertexArray->bind();
    vertexArray->drawUntil(indices.size());
}

void Rx::initializeVertexArray() {

    vertexBuffer = std::make_unique<VertexBuffer>();
    vertexBuffer->setCapacity(vertices.size());

    vertexArray = std::make_unique<VertexArray>();
    vertexArray->setAttribute(0, 13, 3, 0);
    vertexArray->setAttribute(1, 13, 3, 3);
    vertexArray->setAttribute(2, 13, 4, 6);
    vertexArray->setAttribute(3, 13, 2, 10);
    vertexArray->setAttribute(4, 13, 1, 12);
    
    indexBuffer = std::make_unique<IndexBuffer>();
    indexBuffer->setCapacity(indices.size());
}

void Rx::initializeShader() {
    shader = std::make_unique<Shader>();
    shader->setVertexSource(R"(
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

        uniform mat4 uModel;

        void main() {
            gl_Position = uModel * vec4(aPosition, 1.0);

            vNormal = aNormal;
            vColor = aColor;
            vTexCoord = aTexCoord;
            vTexSlot = aTexSlot;
        }
    )");

    shader->setFragmentSource(R"(
        #version 330 core
        
        in vec3 vNormal;
        in vec4 vColor;
        in vec2 vTexCoord;
        in float vTexSlot;

        out vec4 fColor;

        uniform sampler2D uTextures[)" + std::to_string(Texture::getMaxSlot()) + R"(];
        
        void main() {
            fColor = vColor;
        }
    )");

    shader->compile();
}
