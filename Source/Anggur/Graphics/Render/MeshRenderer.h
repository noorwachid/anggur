#pragma once

#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Graphics/Render/Font.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include "Anggur/Graphics/Function.h"

namespace Anggur
{
	struct MeshVertex 
	{
		Vector2 position;
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float textureIndex;
		Vector2 texturePosition;
	};

	class MeshRenderer
	{
	public:
		MeshRenderer() 
		{
			vertices.assign(
				batchVertex, MeshVertex{}
			);

			indices.assign(batchVertex * batchIndexMultiplier, 0);

			textures.assign(TextureSpecification::GetMaxSlot(), nullptr);
			textureIndices.reserve(TextureSpecification::GetMaxSlot());

			for (usize i = 0; i < TextureSpecification::GetMaxSlot(); ++i)
			{
				textureIndices.push_back(i);
			}

			vertexArray.Bind();
			vertexArray.SetLayout({ 
				{ VertexDataType::Float, 2, "aPosition" },
				{ VertexDataType::Float, 4, "aColor" },
				{ VertexDataType::Float, 1, "aTextureIndex" },
				{ VertexDataType::Float, 2, "aTexturePosition" },
			});

			vertexBuffer.Bind();
			vertexBuffer.SetCapacity(sizeof(MeshVertex) * vertices.size());

			indexBuffer.Bind();
			indexBuffer.SetCapacity(sizeof(uint) * indices.size());

			shader.Bind();
			shader.SetVertexSource(R"(
				#version 330 core

				layout (location = 0) in vec2 aPosition;
				layout (location = 1) in vec4 aColor;
				layout (location = 2) in float aTextureIndex;
				layout (location = 3) in vec2 aTexturePosition;

				out vec4 vColor;
				out float vTextureIndex;
				out vec2 vTexturePosition;

				uniform mat3 uView;

				void main() {
					gl_Position.xyz = uView * vec3(aPosition, 1.0f);
					gl_Position.w = 1.0f;

					vColor = aColor;
					vTextureIndex = aTextureIndex;
					vTexturePosition = aTexturePosition;
				}
			)");
			shader.SetFragmentSource(R"(
				#version 330 core
				
				in vec4 vColor;
				in float vTextureIndex;
				in vec2 vTexturePosition;
 
				uniform sampler2D uTextures[)" + std::to_string(TextureSpecification::GetMaxSlot()) + R"(];

				out vec4 fColor;

				void main() 
				{
					int textureIndex = int(vTextureIndex);

					fColor = texture(uTextures[textureIndex], vTexturePosition) * vColor;

					if (fColor.w == 0.0f) {
						discard;
					}
				}
			)");
			shader.Compile();
		}

		void SetView(const Matrix3& newView)
		{
			view = newView;
		}

		void AddTriangle(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color, Texture2D* texture, const Vector2& texturePosition0, const Vector2& texturePosition1, const Vector2& texturePosition2)
		{
			if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > vertices.size() || textureOffset + 1 > textures.size())
			{
				Draw();
			}

			usize textureIndex = 0;

			if (textureIndexMap.count(texture->GetID()))
			{
				textureIndex = textureIndexMap[texture->GetID()];
			}
			else
			{
				textureIndex = textureOffset;
				textureIndexMap[texture->GetID()] = textureIndex;
				textures[textureIndex] = texture;

				++textureOffset;
			}

			vertices[vertexOffset + 0].position = position0;
			vertices[vertexOffset + 1].position = position1;
			vertices[vertexOffset + 2].position = position2;

			vertices[vertexOffset + 0].color = color;
			vertices[vertexOffset + 1].color = color;
			vertices[vertexOffset + 2].color = color;

			vertices[vertexOffset + 0].textureIndex = textureIndex;
			vertices[vertexOffset + 1].textureIndex = textureIndex;
			vertices[vertexOffset + 2].textureIndex = textureIndex;

			vertices[vertexOffset + 0].texturePosition = texturePosition0;
			vertices[vertexOffset + 1].texturePosition = texturePosition1;
			vertices[vertexOffset + 2].texturePosition = texturePosition2;

			indices[indexOffset + 0] = vertexOffset + 0;
			indices[indexOffset + 1] = vertexOffset + 1; 
			indices[indexOffset + 2] = vertexOffset + 2; 

			vertexOffset += 3;

			indexOffset += 3;
		}

		void AddQuad(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3, const Vector4& color, Texture2D* texture, const Vector2& texturePosition0, const Vector2& texturePosition1, const Vector2& texturePosition2, const Vector2& texturePosition3)
		{
			if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > vertices.size() || textureOffset + 1 > textures.size())
			{
				Draw();
			}

			usize textureIndex = 0;

			if (textureIndexMap.count(texture->GetID()))
			{
				textureIndex = textureIndexMap[texture->GetID()];
			}
			else
			{
				textureIndex = textureOffset;
				textureIndexMap[texture->GetID()] = textureIndex;
				textures[textureIndex] = texture;

				++textureOffset;
			}

			vertices[vertexOffset + 0].position = position0;
			vertices[vertexOffset + 1].position = position1;
			vertices[vertexOffset + 2].position = position2;
			vertices[vertexOffset + 3].position = position3;

			vertices[vertexOffset + 0].color = color;
			vertices[vertexOffset + 1].color = color;
			vertices[vertexOffset + 2].color = color;
			vertices[vertexOffset + 3].color = color;

			vertices[vertexOffset + 0].textureIndex = textureIndex;
			vertices[vertexOffset + 1].textureIndex = textureIndex;
			vertices[vertexOffset + 2].textureIndex = textureIndex;
			vertices[vertexOffset + 3].textureIndex = textureIndex;

			vertices[vertexOffset + 0].texturePosition = texturePosition0;
			vertices[vertexOffset + 1].texturePosition = texturePosition1;
			vertices[vertexOffset + 2].texturePosition = texturePosition2;
			vertices[vertexOffset + 3].texturePosition = texturePosition3;

			indices[indexOffset + 0] = vertexOffset + 0;
			indices[indexOffset + 1] = vertexOffset + 1; 
			indices[indexOffset + 2] = vertexOffset + 2; 
			indices[indexOffset + 3] = vertexOffset + 2; 
			indices[indexOffset + 4] = vertexOffset + 3; 
			indices[indexOffset + 5] = vertexOffset + 0;

			vertexOffset += 4;

			indexOffset += 6;
		}

		void AddRectangle(const Vector2& position, const Vector2& size, const Vector4& color, Texture2D* texture, const Vector2& texturePosition, const Vector2& textureSize)
		{
			AddQuad(
				Vector2(position.x,          position.y),
				Vector2(position.x + size.x, position.y),
				Vector2(position.x + size.x, position.y + size.y),
				Vector2(position.x,          position.y + size.y),
				color,
				texture,
				Vector2(texturePosition.x,                 texturePosition.y),
				Vector2(texturePosition.x + textureSize.x, texturePosition.y),
				Vector2(texturePosition.x + textureSize.x, texturePosition.y + textureSize.y),
				Vector2(texturePosition.x,                 texturePosition.y + textureSize.y)
			);
		}

		void Draw()
		{
			// Early exit if no vertices to draw
			if (vertexOffset == 0)
				return;

			for (usize textureIndex = 0; textureIndex < textureOffset; ++textureIndex)
				textures[textureIndex]->Bind(textureIndex);

			shader.Bind();
			shader.SetUniformMatrix3("uView", view);
			shader.SetUniformInt("uTextures", textureOffset, textureIndices.data());

			vertexArray.Bind();

			vertexBuffer.Bind();
			vertexBuffer.SetData(sizeof(MeshVertex) * vertexOffset, vertices.data());

			indexBuffer.Bind();
			indexBuffer.SetData(sizeof(uint) * indexOffset, indices.data());

			glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

			vertexOffset = 0;
			indexOffset = 0;
			textureOffset = 0;

			textureIndexMap.clear();
		}


	private:
		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		Shader shader;

		Matrix3 view;

		std::vector<MeshVertex> vertices;
		std::vector<uint> indices;
		std::vector<Texture2D*> textures;
		std::vector<int> textureIndices;

		std::unordered_map<uint, usize> textureIndexMap;

		usize vertexOffset = 0;
		usize indexOffset = 0;
		usize textureOffset = 0;

		usize drawCount = 0;

		usize batchVertex = 512;
		usize batchIndexMultiplier = 2;
	};
}


