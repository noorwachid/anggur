#include "Renderer.h"
#include <Anggur/Math/Matrix3.h>
#include <iostream>

Renderer::Renderer() {
    initializeVertexArray();
    initializeShader();

    blankTextureData = {255, 255, 255, 255};
    blankTexture = std::make_shared<Texture>(blankTextureData.data(), 1, 1, 4);

    textures.reserve(Texture::getMaxSlot());
}

Renderer::~Renderer() {
}

void Renderer::initializeVertexArray() {

    vertices.assign(batchVertex, Vertex(Vector2(0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f));
    indices.assign(batchVertex * batchIndexMultiplier, 0);

    vertexBuffer.setCapacity(sizeof(Vertex) * vertices.size());

    vertexArray.setAttribute(0, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
    vertexArray.setAttribute(1, 4, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
    vertexArray.setAttribute(2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoord));
    vertexArray.setAttribute(3, 1, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texSlot));
    
    indexBuffer.setCapacity(sizeof(uint32_t) * indices.size());
}

void Renderer::initializeShader() {
    shader.setVertexSource(R"(
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

    shader.setFragmentSource(R"(
        #version 330 core
        
        in vec4 vColor;
        in vec2 vTexCoord;
        in float vTexSlot;

        out vec4 fColor;

        uniform sampler2D uTextures[)" + std::to_string(Texture::getMaxSlot()) + R"(];
        
        void main() {
            fColor = texture(uTextures[int(vTexSlot)], vTexCoord);
        }
    )");

    shader.compile();
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

void Renderer::begin() {
    renderCount = 0;

    textures.push_back(blankTexture);
}

void Renderer::end() {
    Renderer::flush();
}

void Renderer::flush() {
    if (vertexOffset == 0) {
        return;
    }

    shader.bind();
    shader.setUniformMatrix3("uViewProjection", viewProjection);

    vertexArray.bind();
    
    vertexBuffer.bind();
    vertexBuffer.setData(sizeof(Vertex) * vertices.size(), vertices.data());

    indexBuffer.bind();
    indexBuffer.setData(sizeof(uint32_t) * indices.size(), indices.data());

    for (auto& texture: textures) {
        texture->bind();
    }

    glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

    ++renderCount;

    vertexOffset = 0;
    indexOffset = 0;
	textureOffset = 0;

    textures.clear();
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

void Renderer::renderTexturedRectangle(const Vector2& position, const Vector2& size, const std::shared_ptr<Texture>& texture, const Vector2& texturePosition, const Vector2& textureSize, const Vector4& color) {
	float textureSlot = static_cast<float>(textureOffset);

    render({
        // position                                               // color                                     // texCoord          // texSlot
        Vertex(Vector2(position.x,          position.y),          Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x,                 texturePosition.y                ), textureSlot),
        Vertex(Vector2(position.x + size.x, position.y),	      Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x + textureSize.x, texturePosition.y                ), textureSlot),
        Vertex(Vector2(position.x,          position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x,                 texturePosition.y + textureSize.y), textureSlot),
        Vertex(Vector2(position.x + size.x, position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x + textureSize.x, texturePosition.y + textureSize.y), textureSlot),
    }, {
        0, 1, 2,
        2, 3, 1,
    });

	++textureOffset;
}
