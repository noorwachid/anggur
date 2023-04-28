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
	struct TextVertex 
	{
		Vector2 position;
		Vector2 texturePosition;
		float textureIndex = 0.0f;
		float thickness = 0.5f;
		float sharpness = 0.01f;
		float scale = 1.0f;
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class TextRenderer
	{
	public:
		TextRenderer() 
		{
			vertices.assign(batchVertex, TextVertex{});

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
				{ VertexDataType::Float, 2, "aTexturePosition" },
				{ VertexDataType::Float, 1, "aTextureIndex" },
				{ VertexDataType::Float, 1, "aThickness" },
				{ VertexDataType::Float, 1, "aSharpness" },
				{ VertexDataType::Float, 1, "aScale" },
				{ VertexDataType::Float, 4, "aColor" },
			});

			vertexBuffer.Bind();
			vertexBuffer.SetCapacity(vertexArray.GetStride() * vertices.size());

			indexBuffer.Bind();
			indexBuffer.SetCapacity(sizeof(uint) * indices.size());

			shader.Bind();
			shader.SetVertexSource(R"(
				#version 330 core

				layout (location = 0) in vec2 aPosition;
				layout (location = 1) in vec2 aTexturePosition;
				layout (location = 2) in float aTextureIndex;
				layout (location = 3) in float aThickness;
				layout (location = 4) in float aSharpness;
				layout (location = 5) in float aScale;
				layout (location = 6) in vec4 aColor;

				out vec2 vTexturePosition;
				out float vTextureIndex;
				out float vThickness;
				out float vSharpness;
				out float vScale;
				out vec4 vColor;

				uniform mat3 uView;

				void main() 
				{
					gl_Position.xyz = uView * vec3(aPosition, 1.0f);
					gl_Position.w = 1.0f;

					vTexturePosition = aTexturePosition;
					vTextureIndex = aTextureIndex;
					vThickness = aThickness;
					vSharpness = aSharpness;
					vScale = aScale;
					vColor = aColor;
				}
			)");
			shader.SetFragmentSource(R"(
				#version 330 core
				
				in vec2 vTexturePosition;
				in float vTextureIndex;
				in float vThickness;
				in float vSharpness;
				in float vScale;
				in vec4 vColor;

				out vec4 fColor;

				uniform sampler2D uTextures[)" + std::to_string(TextureSpecification::GetMaxSlot()) + R"(];

				void main() 
				{
					int textureIndex = int(vTextureIndex);
					float sample = texture(uTextures[textureIndex], vTexturePosition).r;
					float thickness = 0.95;

					fColor = vColor;
					fColor.w *= smoothstep(thickness - vSharpness, thickness, sample);

					if (fColor.w == 0.0f) 
					{
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

		void AddLine(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, const Vector4& color)
		{
			float padding = 1.0f / font->sampleSize * font->samplePadding * size;

			Vector2 pointer(-padding, 0);

			for (usize i = 0; i < content.size(); ++i)
			{
				if (content[i] == ' ')
				{
					pointer.x += size * font->GetSpaceWidth();
					continue;
				}

				if (font->glyphMap.count(content[i]) == 0)
				{
					pointer.x += size;
					continue;
				}

				const FontGlyph& glyph = font->glyphMap[content[i]];

				Vector2 localPosition = size * glyph.position;
				Vector2 localSize = size * glyph.size;

				AddCharacter(pointer + position + localPosition, localSize , thickness, sharpness, 1, color, font->textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize);

				pointer.x += localPosition.x + localSize.x - (padding * 2) + (size * font->GetKerning(content[i], content[i + 1]));
			}
		}

		void AddCharacter(const Vector2& position, const Vector2& size, float thickness, float sharpness, float scale, const Vector4& color, Texture2D* texture, const Vector2& texturePosition, const Vector2& textureSize)
		{
			if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > indices.size() || textureOffset + 1 > textures.size())
			{
				Draw();
			}

			vertices[vertexOffset + 0].position.Set(position.x,          position.y);
			vertices[vertexOffset + 1].position.Set(position.x + size.x, position.y);
			vertices[vertexOffset + 2].position.Set(position.x + size.x, position.y + size.y);
			vertices[vertexOffset + 3].position.Set(position.x,          position.y + size.y);

			vertices[vertexOffset + 0].thickness = thickness;
			vertices[vertexOffset + 1].thickness = thickness;
			vertices[vertexOffset + 2].thickness = thickness;
			vertices[vertexOffset + 3].thickness = thickness;

			vertices[vertexOffset + 0].sharpness = sharpness;
			vertices[vertexOffset + 1].sharpness = sharpness;
			vertices[vertexOffset + 2].sharpness = sharpness;
			vertices[vertexOffset + 3].sharpness = sharpness;

			vertices[vertexOffset + 0].scale = scale;
			vertices[vertexOffset + 1].scale = scale;
			vertices[vertexOffset + 2].scale = scale;
			vertices[vertexOffset + 3].scale = scale;

			vertices[vertexOffset + 0].color = color;
			vertices[vertexOffset + 1].color = color;
			vertices[vertexOffset + 2].color = color;
			vertices[vertexOffset + 3].color = color;

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

			vertices[vertexOffset + 0].textureIndex = textureIndex;
			vertices[vertexOffset + 1].textureIndex = textureIndex;
			vertices[vertexOffset + 2].textureIndex = textureIndex;
			vertices[vertexOffset + 3].textureIndex = textureIndex;

			vertices[vertexOffset + 0].texturePosition.Set(texturePosition.x,                 texturePosition.y);
			vertices[vertexOffset + 1].texturePosition.Set(texturePosition.x + textureSize.x, texturePosition.y);
			vertices[vertexOffset + 2].texturePosition.Set(texturePosition.x + textureSize.x, texturePosition.y + textureSize.y);
			vertices[vertexOffset + 3].texturePosition.Set(texturePosition.x,                 texturePosition.y + textureSize.y);

			indices[indexOffset + 0] = vertexOffset + 0;
			indices[indexOffset + 1] = vertexOffset + 1; 
			indices[indexOffset + 2] = vertexOffset + 2; 
			indices[indexOffset + 3] = vertexOffset + 2; 
			indices[indexOffset + 4] = vertexOffset + 3; 
			indices[indexOffset + 5] = vertexOffset + 0;

			vertexOffset += 4;

			indexOffset += 6;
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
			vertexBuffer.SetData(vertexArray.GetStride() * vertexOffset, vertices.data());

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

		std::vector<TextVertex> vertices;
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


