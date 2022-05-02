#include "Renderer.h"
#include <Anggur/Math/Matrix3.h>
#include <iostream>

std::unique_ptr<Shader> Renderer::shader;
std::unique_ptr<VertexArray> Renderer::vertexArray;
std::unique_ptr<VertexBuffer> Renderer::vertexBuffer;
std::unique_ptr<IndexBuffer> Renderer::indexBuffer;
std::vector<Renderer::Vertex> Renderer::vertices;
std::vector<uint32_t> Renderer::indices;

size_t Renderer::vertexOffset = 0;
size_t Renderer::indexOffset = 0;
size_t Renderer::renderCount = 0;
size_t Renderer::batchVertex = 128;
size_t Renderer::batchIndexMultiplier = 2;

Matrix3 Renderer::viewProjection;

void Renderer::initialize() {
    initializeVertexArray();
    initializeShader();
}

void Renderer::setClearColor(const Vector4& color) {
    glClearColor(color.x, color.y, color.z, color.w);
}

void Renderer::setBatchChunk(size_t vertex, size_t indexMultiplier) {
    batchVertex = vertex;
    batchIndexMultiplier = indexMultiplier;
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::setViewProjection(const Matrix3& newViewProjection) {
    viewProjection = newViewProjection;
}

void Renderer::beginScene() {
    renderCount = 0;
}

void Renderer::endScene() {
    Renderer::flush();
}

void Renderer::flush() {
    if (vertexOffset == 0) {
        return;
    }

    shader->bind();
    shader->setUniformMatrix3("uViewProjection", viewProjection);
    
    vertexBuffer->bind();
    vertexBuffer->setData(sizeof(Vertex) * vertices.size(), vertices.data());

    indexBuffer->bind();
    indexBuffer->setData(sizeof(uint32_t) * indices.size(), indices.data());

    vertexArray->bind();

    glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

    ++renderCount;

    vertexOffset = 0;
    indexOffset = 0;
}

void Renderer::initializeVertexArray() {

    vertices.assign(batchVertex, Vertex(Vector2(0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f));
    indices.assign(batchVertex * batchIndexMultiplier, 0);

    vertexBuffer = std::make_unique<VertexBuffer>();
    vertexBuffer->setCapacity(sizeof(Vertex) * vertices.size());

    vertexArray = std::make_unique<VertexArray>();
    vertexArray->setAttribute(0, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
    vertexArray->setAttribute(1, 4, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
    vertexArray->setAttribute(2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoord));
    vertexArray->setAttribute(3, 1, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texSlot));
    
    indexBuffer = std::make_unique<IndexBuffer>();
    indexBuffer->setCapacity(sizeof(uint32_t) * indices.size());
}

void Renderer::initializeShader() {
    shader = std::make_unique<Shader>();
    shader->setVertexSource(R"(
        #version 330 core

        layout (location = 0) in vec2 aPosition;
        layout (location = 1) in vec4 aColor;
        layout (location = 2) in vec2 aTexCoord;
        layout (location = 3) in float aTexSlot;

        out vec4 vColor;
        out vec2 vTexCoord;
        out float vTexSlot;

        uniform mat3 uViewProjection;

        void main() {
            gl_Position = vec4(uViewProjection * vec3(aPosition, 1.0f), 1.0f);

            vColor = aColor;
            vTexCoord = aTexCoord;
            vTexSlot = aTexSlot;
        }
    )");

    shader->setFragmentSource(R"(
        #version 330 core
        
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


void Renderer::render(const std::vector<Vertex>& newVertices, const std::vector<uint32_t>& newIndices) {
    if (vertexOffset + newVertices.size() > vertices.size() || indexOffset + newIndices.size() > indices.size()) {
        flush();
    }

    for (size_t i = 0; i < newVertices.size(); ++i) {
        vertices[i + vertexOffset] = newVertices[i];
    }

    for (size_t i = 0; i < newIndices.size(); ++i) {
        indices[i + indexOffset] = newIndices[i] + vertexOffset;
    }

    vertexOffset += newVertices.size();
    indexOffset += newIndices.size();
}


void Renderer::renderRectangle(const Vector2& position, const Vector2& size, const Vector4& color) {
    render({
        // position                                               // color                                     // texCoord          // texSlot
        Vertex(Vector2(position.x,          position.y),          Vector4(color.x, color.y, color.z, color.w), Vector2(0.0f, 0.0f), 0.0f),
        Vertex(Vector2(position.x + size.x, position.y),	      Vector4(color.x, color.y, color.z, color.w), Vector2(1.0f, 0.0f), 0.0f),
        Vertex(Vector2(position.x,          position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(0.0f, 1.0f), 0.0f),
        Vertex(Vector2(position.x + size.x, position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(1.0f, 1.0f), 0.0f),
    }, {
        0, 1, 2,
        2, 3, 1,
    });
}