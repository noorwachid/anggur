#include "Function.h"
#include "VertexArray.h"
#include "Shader.h"
#include "MeshRenderer.h"
#include "Anggur/Math/Matrix4.h"
#include <vector>
#include <iostream>

namespace Anggur 
{
    MeshRenderer::MeshRenderer() {
        InitializeVertexPool();
        InitializeTexturePool();
        InitializeShader();

        glEnable(GL_DEPTH_TEST);
    }

    MeshRenderer::~MeshRenderer() {
    }

    void MeshRenderer::InitializeVertexPool() {
        vertices.assign(batchVertex, MeshVertex(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f));
        indices.assign(batchVertex * batchIndexMultiplier, 0);

        vertexBuffer = std::make_shared<VertexBuffer>();
        vertexBuffer->setCapacity(sizeof(MeshVertex) * vertices.size());

        vertexArray = std::make_shared<VertexArray>();
        vertexArray->setAttribute(0, 3, GL_FLOAT, sizeof(MeshVertex), offsetof(MeshVertex, position));
        vertexArray->setAttribute(1, 3, GL_FLOAT, sizeof(MeshVertex), offsetof(MeshVertex, normal));
        vertexArray->setAttribute(2, 4, GL_FLOAT, sizeof(MeshVertex), offsetof(MeshVertex, color));
        vertexArray->setAttribute(3, 2, GL_FLOAT, sizeof(MeshVertex), offsetof(MeshVertex, uv));
        vertexArray->setAttribute(4, 1, GL_FLOAT, sizeof(MeshVertex), offsetof(MeshVertex, slot));
        
        indexBuffer = std::make_shared<IndexBuffer>();
        indexBuffer->setCapacity(sizeof(uint32_t) * indices.size());
    }

    void MeshRenderer::InitializeTexturePool() {
        textures.assign(Texture::getMaxSlot(), nullptr);
        textureSlots.reserve(Texture::getMaxSlot());

        for (int i = 0; i < Texture::getMaxSlot(); ++i) 
        {
            textureSlots.push_back(i);
        }

        uint8_t whitePixel[] = {255, 255, 255, 255};
        whiteTexture = std::make_shared<Texture2D>(whitePixel, 1, 1, 4);
    }

    void MeshRenderer::InitializeShader() {
        shader = std::make_shared<Shader>();
        shader->setVertexSource(R"(
            #version 330 core

            layout (location = 0) in vec3 aPosition;
            layout (location = 1) in vec3 aNormal;
            layout (location = 2) in vec4 aColor;
            layout (location = 3) in vec2 aTexCoord;
            layout (location = 4) in float aTexSlot;

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

        shader->setFragmentSource(R"(
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

        shader->compile();
    }

    void MeshRenderer::setBatchChunk(size_t vertex, size_t indexMultiplier) {
        batchVertex = vertex;
        batchIndexMultiplier = indexMultiplier;
    }

    void MeshRenderer::clear(const Vector4& color) {
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }

    void MeshRenderer::setViewport(const Vector2& size) {
        glViewport(0, 0, size.x, size.y);
    }

    void MeshRenderer::setViewport(const Vector2& position, const Vector2& size) {
        glViewport(position.x, position.y, size.x, size.y);
    }

    void MeshRenderer::setViewProjection(const Matrix4& newViewProjection)
    {
        viewProjection = newViewProjection;
    }

    void MeshRenderer::begin() {
        renderCount = 0;

        flushInternalBuffer();
    }

    void MeshRenderer::begin(const Matrix4& viewProjection) {
        setViewProjection(viewProjection);
        begin();
    }

    void MeshRenderer::end() {
        flush();
    }

    void MeshRenderer::flush() {
        if (vertexOffset == 0) 
            return;

        for (size_t i = 0; i < textureOffset; ++i) 
            textures[i]->bind(i);

        shader->bind();
        shader->setUniformMatrix4("uViewProjection", viewProjection);
        shader->setUniformInt("uTextures", textureOffset, textureSlots.data());

        vertexArray->bind();
        
        vertexBuffer->bind();
        vertexBuffer->setData(sizeof(MeshVertex) * vertexOffset, vertices.data());

        indexBuffer->bind();
        indexBuffer->setData(sizeof(uint32_t) * indexOffset, indices.data());

        glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

        flushInternalBuffer();

        ++renderCount;
    }

    void MeshRenderer::flushInternalBuffer() {
        vertexOffset = 0;
        indexOffset = 0;
        textureOffset = 0;
    }

    bool MeshRenderer::isCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize) {
        return 
            vertexOffset + newVertexSize > vertices.size() ||
            indexOffset + newIndexSize > indices.size() ||
            textureOffset + newTextureSize > textures.size();
    }

    void MeshRenderer::draw(const std::vector<MeshVertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture) {
        if (isCapacityMaxout(newVertices.size(), newIndices.size(), 1)) {
            flush();
        }

        // Find or add new texture slot 
        int textureSlot = 0;

        // This code only create one branch
        for (; textureSlot < textureOffset && textures[textureSlot]->getID() != texture->getID(); ++textureSlot);

        if (textureSlot == textureOffset) {
            textureSlot = textureOffset;
            textures[textureOffset] = texture;
            textureOffset += 1;
        }

        for (size_t i = 0; i < newVertices.size(); ++i) {
            auto& vertex = vertices[i + vertexOffset];

            vertex = newVertices[i];
            vertex.slot = textureSlot;
        }

        for (size_t i = 0; i < newIndices.size(); ++i) {
            indices[i + indexOffset] = newIndices[i] + vertexOffset;
        }

        vertexOffset += newVertices.size();
        indexOffset += newIndices.size();
    }

    void MeshRenderer::drawCube(const Matrix4& model, const Vector4& color)
    {
        draw(
            {
                // position                                 // normal                  // color                                     // texCoord         
                MeshVertex(model * Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                MeshVertex(model * Vector3( 0.5f, -0.5f, -0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector4(color.x, color.y, color.z, color.w), Vector2(1, 0)),
                MeshVertex(model * Vector3( 0.5f,  0.5f, -0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector4(color.x, color.y, color.z, color.w), Vector2(1, 1)),
                MeshVertex(model * Vector3(-0.5f,  0.5f, -0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector4(color.x, color.y, color.z, color.w), Vector2(0, 1)),
                 
                MeshVertex(model * Vector3( 0.5f, -0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                MeshVertex(model * Vector3(-0.5f, -0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector4(color.x, color.y, color.z, color.w), Vector2(1, 0)),
                MeshVertex(model * Vector3(-0.5f,  0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector4(color.x, color.y, color.z, color.w), Vector2(1, 1)),
                MeshVertex(model * Vector3( 0.5f,  0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector4(color.x, color.y, color.z, color.w), Vector2(0, 1)),
            }, {
                // front
                0, 1, 2,
                2, 3, 0,

                // back
                4, 5, 6,
                6, 7, 4,

                // left
                5, 0, 3,
                3, 6, 5,

                // right
                1, 4, 7,
                7, 2, 1,

                // top
                3, 2, 7,
                7, 6, 3,

                // bottom
                1, 0, 5,
                5, 4, 1,
            }, 
            whiteTexture
        );
    }
}