#include "MeshRenderer.h"
#include "Anggur/Math/Matrix4.h"
#include "Function.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include <iostream>
#include <vector>

namespace Anggur
{
	MeshRenderer::MeshRenderer()
	{
		InitializeVertexPool();
		InitializeTexturePool();
		InitializeShader();

		glEnable(GL_DEPTH_TEST);
	}

	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::InitializeVertexPool()
	{
		vertices.assign(batchVertex, Vertex(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f),
												Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f));
		indices.assign(batchVertex * batchIndexMultiplier, 0);

		vertexBuffer = std::make_shared<VertexBuffer>();
		vertexBuffer->SetCapacity(sizeof(Vertex) * vertices.size());

		vertexArray = std::make_shared<VertexArray>();
		vertexArray->SetAttribute(0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
		vertexArray->SetAttribute(1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal));
		vertexArray->SetAttribute(2, 4, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
		vertexArray->SetAttribute(3, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, uv));
		vertexArray->SetAttribute(4, 1, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, slot));

		indexBuffer = std::make_shared<IndexBuffer>();
		indexBuffer->SetCapacity(sizeof(uint32_t) * indices.size());
	}

	void MeshRenderer::InitializeTexturePool()
	{
		textures.assign(Texture::GetMaxSlot(), nullptr);
		textureSlots.reserve(Texture::GetMaxSlot());

		for (int i = 0; i < Texture::GetMaxSlot(); ++i)
		{
			textureSlots.push_back(i);
		}

		std::vector<uint8_t> bytes = {{255, 255, 255, 255}};
		whiteTexture = std::make_shared<Texture2D>(bytes, 1, 1, 4);
	}

	void MeshRenderer::InitializeShader()
	{
		shader = std::make_shared<Shader>();
		shader->SetVertexSource(R"(
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

		shader->SetFragmentSource(R"(
            #version 330 core
            
            in vec4 vColor;
            in vec2 vTexCoord;
            in float vTexSlot;

            out vec4 fColor;

            uniform sampler2D uTextures[)" +
								  std::to_string(Texture::GetMaxSlot()) + R"(];
            
            void main() {
                fColor = texture(uTextures[int(vTexSlot)], vTexCoord) * vColor;
            }
        )");

		shader->Compile();
	}

	void MeshRenderer::SetBatchChunk(size_t vertex, size_t indexMultiplier)
	{
		batchVertex = vertex;
		batchIndexMultiplier = indexMultiplier;
	}

	void MeshRenderer::Clear(const Vector4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void MeshRenderer::SetViewport(const Vector2& size)
	{
		glViewport(0, 0, size.x, size.y);
	}

	void MeshRenderer::SetViewport(const Vector2& position, const Vector2& size)
	{
		glViewport(position.x, position.y, size.x, size.y);
	}

	void MeshRenderer::SetViewProjection(const Matrix4& newViewProjection)
	{
		viewProjection = newViewProjection;
	}

	void MeshRenderer::BeginScene()
	{
		drawCount = 0;

		FlushInternalBuffer();
	}

	void MeshRenderer::BeginScene(const Matrix4& viewProjection)
	{
		SetViewProjection(viewProjection);
		BeginScene();
	}

	void MeshRenderer::EndScene()
	{
		Flush();
	}

	void MeshRenderer::Flush()
	{
		if (vertexOffset == 0)
			return;

		for (size_t i = 0; i < textureOffset; ++i)
			textures[i]->Bind(i);

		shader->Bind();
		shader->SetUniformMatrix4("uViewProjection", viewProjection);
		shader->SetUniformInt("uTextures", textureOffset, textureSlots.data());

		vertexArray->Bind();

		vertexBuffer->Bind();
		vertexBuffer->setData(sizeof(Vertex) * vertexOffset, vertices.data());

		indexBuffer->Bind();
		indexBuffer->setData(sizeof(uint32_t) * indexOffset, indices.data());

		glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

		FlushInternalBuffer();

		++drawCount;
	}

	void MeshRenderer::FlushInternalBuffer()
	{
		vertexOffset = 0;
		indexOffset = 0;
		textureOffset = 0;
	}

	bool MeshRenderer::IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize)
	{
		return vertexOffset + newVertexSize > vertices.size() || indexOffset + newIndexSize > indices.size() ||
			   textureOffset + newTextureSize > textures.size();
	}

	void MeshRenderer::Draw(const Matrix4& model, const Mesh& mesh)
	{
		if (IsCapacityMaxout(mesh.vertices.size(), mesh.indices.size(), 1))
			Flush();

        int textureSlot = 0;

        if (!mesh.textures.empty())
        {
            for (const auto& texture : mesh.textures)
            {
                // Find or add new texture slot
                // This code only create one branch
                for (; textureSlot < textureOffset && textures[textureSlot]->GetID() != texture->GetID(); ++textureSlot)
                    ;

                if (textureSlot == textureOffset)
                {
                    textureSlot = textureOffset;
                    textures[textureOffset] = texture;
                    textureOffset += 1;
                }
            }
        } 
        else 
        {
            for (; textureSlot < textureOffset && textures[textureSlot]->GetID() != whiteTexture->GetID(); ++textureSlot)
                ;

            if (textureSlot == textureOffset)
            {
                textureSlot = textureOffset;
                textures[textureOffset] = whiteTexture;
                textureOffset += 1;
            }
        }


		for (size_t i = 0; i < mesh.vertices.size(); ++i)
		{
			auto& vertex = vertices[i + vertexOffset];

			vertex = mesh.vertices[i];
            vertex.position = model * vertex.position;
			vertex.slot = textureSlot;
		}

		for (size_t i = 0; i < mesh.indices.size(); ++i)
		{
			indices[i + indexOffset] = mesh.indices[i] + vertexOffset;
		}

		vertexOffset += mesh.vertices.size();
		indexOffset += mesh.indices.size();
	}
}
