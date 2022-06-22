#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Shader.h>
#include "Renderer.h"
#include <vector>

struct RendererData {
	std::shared_ptr<Shader> shader;
	std::shared_ptr<VertexArray> vertexArray;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;

	std::vector<Renderer::Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<std::shared_ptr<Texture2D>> textures;
    std::vector<int> textureSlots;

    std::shared_ptr<Texture2D> whiteTexture;

	size_t vertexOffset = 0;
	size_t indexOffset = 0;
	size_t textureOffset = 0;

	size_t renderCount = 0;
	size_t batchVertex = 128;
	size_t batchIndexMultiplier = 2;

	Matrix4 viewProjection;
};

static RendererData data;

Renderer::Renderer() {
}

void Renderer::initialize() {
    initializeVertexPool();
    initializeTexturePool();
    initializeShader();

    Vector4 color = Vector4::charcoal;

    glEnable(GL_DEPTH_TEST);
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void Renderer::initializeVertexPool() {
    data.vertices.assign(data.batchVertex, Vertex(Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f));
    data.indices.assign(data.batchVertex * data.batchIndexMultiplier, 0);

    data.vertexBuffer = std::make_shared<VertexBuffer>();
    data.vertexBuffer->setCapacity(sizeof(Vertex) * data.vertices.size());

    data.vertexArray = std::make_shared<VertexArray>();
    data.vertexArray->setAttribute(0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
    data.vertexArray->setAttribute(1, 4, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
    data.vertexArray->setAttribute(2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, textureCoord));
    data.vertexArray->setAttribute(3, 1, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, textureSlot));
    
    data.indexBuffer = std::make_shared<IndexBuffer>();
    data.indexBuffer->setCapacity(sizeof(uint32_t) * data.indices.size());
}

void Renderer::initializeTexturePool() {
    data.textures.assign(Texture::getMaxSlot(), nullptr);
    data.textureSlots.reserve(Texture::getMaxSlot());

    for (int i = 0; i < Texture::getMaxSlot(); ++i) {
        data.textureSlots.push_back(i);
    }

    uint8_t whitePixel[] = {255, 255, 255, 255};
    data.whiteTexture = std::make_shared<Texture2D>(whitePixel, 1, 1, 4);
}

void Renderer::initializeShader() {
    data.shader = std::make_shared<Shader>();
    data.shader->setVertexSource(R"(
        #version 330 core

        layout (location = 0) in vec3 aPosition;
        layout (location = 1) in vec4 aColor;
        layout (location = 2) in vec2 aTexCoord;
        layout (location = 3) in float aTexSlot;

        out vec4 vColor;
        out vec2 vTexCoord;
        out float vTexSlot;

        uniform mat4 uViewProjection;

        void main() {
            gl_Position = uViewProjection * vec4(aPosition, 1.0f);

            vColor = aColor;
            vTexCoord = aTexCoord;
            vTexSlot = aTexSlot;
        }
    )");

    data.shader->setFragmentSource(R"(
        #version 330 core
        
        in vec4 vColor;
        in vec2 vTexCoord;
        in float vTexSlot;

        out vec4 fColor;

        uniform sampler2D uTextures[)" + std::to_string(Texture::getMaxSlot()) + R"(];
        
        void main() {
            fColor = texture(uTextures[int(vTexSlot)], vTexCoord) * vColor;
        }
    )");

    data.shader->compile();
}

void Renderer::setBatchChunk(size_t vertex, size_t indexMultiplier) {
    data.batchVertex = vertex;
    data.batchIndexMultiplier = indexMultiplier;
}

void Renderer::clear(const Vector4& color) {
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}


void Renderer::setViewport(const Vector2& position, const Vector2& size) {
    glViewport(position.x, position.y, size.x, size.y);
}

void Renderer::begin() {
    data.renderCount = 0;

    flushData();
}

void Renderer::begin(const Matrix4& newViewProjection) {
    data.viewProjection = newViewProjection;

    begin();
}

void Renderer::end() {
    flush();
}

void Renderer::flush() {
    if (data.vertexOffset == 0) {
        return;
    }

    for (size_t i = 0; i < data.textureOffset; ++i) {
        data.textures[i]->bind(i);
    }

    data.shader->bind();
    data.shader->setUniformMatrix4("uViewProjection", data.viewProjection);
    data.shader->setUniformInt("uTextures", data.textureOffset, data.textureSlots.data());

    data.vertexArray->bind();
    
    data.vertexBuffer->bind();
    data.vertexBuffer->setData(sizeof(Vertex) * data.vertexOffset, data.vertices.data());

    data.indexBuffer->bind();
    data.indexBuffer->setData(sizeof(uint32_t) * data.indexOffset, data.indices.data());

    glDrawElements(GL_TRIANGLES, data.indexOffset, GL_UNSIGNED_INT, nullptr);

    flushData();

    ++data.renderCount;
}

void Renderer::flushData() {
    data.vertexOffset = 0;
    data.indexOffset = 0;
	data.textureOffset = 0;
}

bool Renderer::isCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize) {
    return 
        data.vertexOffset + newVertexSize > data.vertices.size() ||
        data.indexOffset + newIndexSize > data.indices.size() ||
        data.textureOffset + newTextureSize > data.textures.size();
}

void Renderer::render(const std::vector<Vertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture) {
    if (isCapacityMaxout(newVertices.size(), newIndices.size(), 1)) {
        flush();
    }

    // Find or add new texture slot 
    int textureSlot = 0;

    // This code only create one branch
    for (; textureSlot < data.textureOffset && data.textures[textureSlot]->getId() != texture->getId(); ++textureSlot);

    if (textureSlot == data.textureOffset) {
        textureSlot = data.textureOffset;
        data.textures[data.textureOffset] = texture;
        data.textureOffset += 1;
    }

    for (size_t i = 0; i < newVertices.size(); ++i) {
        auto& vertex = data.vertices[i + data.vertexOffset];

        vertex = newVertices[i];
        vertex.textureSlot = textureSlot;
    }

    for (size_t i = 0; i < newIndices.size(); ++i) {
        data.indices[i + data.indexOffset] = newIndices[i] + data.vertexOffset;
    }

    data.vertexOffset += newVertices.size();
    data.indexOffset += newIndices.size();
}

void Renderer::renderCube() {
    Vector3 position(0.0f, 0.0f, 0.1f);
    float r = 0.3;

    render(
        {
            Vertex(Vector3(position.x - r, position.y - r, position.z), Vector4(1.0f, 0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f)),
            Vertex(Vector3(position.x + r, position.y - r, position.z),	Vector4(1.0f, 1.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f)),
            Vertex(Vector3(position.x + r, position.y + r, position.z), Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f)),
            Vertex(Vector3(position.x - r, position.y + r, position.z), Vector4(0.0f, 0.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f)),
        }, {
            0, 1, 2,
            2, 3, 0,
        }, 
        data.whiteTexture
    );
}