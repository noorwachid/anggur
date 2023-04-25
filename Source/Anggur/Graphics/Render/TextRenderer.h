#pragma once

#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Graphics/Render/Font.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include "Anggur/Graphics/Function.h"
#include "msdfgen.h"
#include "msdfgen-ext.h"

namespace Anggur
{
	struct TextCharacterInformation
	{
		Vector2 position;
		Vector2 size;

		usize textureIndex = 0;
		Vector2 texturePosition;
		Vector2 textureSize;
	};

	class TextManager
	{
	};

	class TextFont
	{
	public:
		usize atlasPx = 128;
		usize characterPx = 24;
		usize marginPx = 2;

		float edgeColoringAngle = 8.0f;
		float distanceRange = 2.0f;

		float padding = 0.0f;
		float scale = 0.0f;

		usize hotTextureIndex = 0;
		std::vector<Texture2D*> textures;
		std::vector<uchar> hotBytes;

		std::unordered_map<uint, TextCharacterInformation> characterInformationMap;

		msdfgen::FontHandle* handle;

		const TextCharacterInformation& GetCharacterInformation(uint character)
		{
			return characterInformationMap[character];
		}

		float GetKerning(uint a, uint b)
		{
			double kerning = 0;

			msdfgen::getKerning(kerning, handle, a, b);

			return kerning * scale;
		}

		void Load(const std::string& path)
		{
			msdfgen::FreetypeHandle *ft = msdfgen::initializeFreetype();
			if (ft) {
				handle = msdfgen::loadFont(ft, path.c_str());
				msdfgen::FontMetrics metrics;
				msdfgen::getFontMetrics(metrics, handle);

				float pixelScale = 1.0f / metrics.emSize;

				padding = pixelScale * distanceRange;

				if (handle) 
				{
					int channels = 3;
					std::vector<char> characters = 
					{ 
						'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', ':', ';', '?', '!', '-', '(', ')', '{', '}', '[', ']', '<', '>',
						'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
						'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
					};
					hotBytes.assign(channels * atlasPx * atlasPx, 0);

					usize yOffsetMaxPx = 0;
					usize xOffsetPx = marginPx;
					usize yOffsetPx = marginPx;

					for (usize i = 0; i < characters.size(); ++i) 
					{
						msdfgen::Shape shape;

						char character = characters[i];
						
						if (!msdfgen::loadGlyph(shape, handle, character)) 
						{
							continue;
						}

						// shape.inverseYAxis = true;
						shape.normalize();

						auto bounds = shape.getBounds();

						Vector2 size(
							Math::Abs(bounds.l - bounds.r) + distanceRange,
							Math::Abs(bounds.b - bounds.t) + distanceRange
						);

						Vector2 subbytesSize = size * characterPx * pixelScale;

						msdfgen::Bitmap<float, 3> subbytes(subbytesSize.x, subbytesSize.y);

						msdfgen::edgeColoringSimple(shape, edgeColoringAngle);

						msdfgen::generateMSDF(subbytes, shape, distanceRange, 
							msdfgen::Vector2(characterPx * pixelScale, characterPx * pixelScale),
							msdfgen::Vector2(
								((distanceRange * 0.5) - bounds.l),
								((distanceRange * 0.5) - bounds.b)
								// 0
							)
						);

						float* rawSubbytes = static_cast<float*>(subbytes);

						if (xOffsetPx + subbytes.width() > atlasPx)
						{
							xOffsetPx = marginPx;
							yOffsetPx += yOffsetMaxPx + marginPx;
							yOffsetMaxPx = 0;
						}

						if (yOffsetPx + subbytes.height() > atlasPx)
						{
							Texture2D* texture = new Texture2D();
							texture->Read(hotBytes, atlasPx, atlasPx, channels, SamplerFilter::Linear);
							textures.push_back(texture);
							++hotTextureIndex;

							hotBytes.assign(atlasPx * atlasPx * channels, 0);

							xOffsetPx = marginPx;
							yOffsetPx = marginPx;
							yOffsetMaxPx = 0;
						}

						for (usize y = 0; y < subbytes.height(); ++y) 
						{
							for (usize x = 0; x < subbytes.width(); ++x)
							{
								for (usize c = 0; c < channels; ++c)
								{
									float hdr = rawSubbytes[y * subbytes.width() * channels + x * channels + c];
									char normal = Math::Clamp(255 * hdr, 0, 255);
									hotBytes[(y + yOffsetPx) * atlasPx * channels + (x + xOffsetPx) * channels + c] = normal;
								}
							}
						}

						scale = 1.0f / metrics.emSize;
						float atlasScale = 1.0f / atlasPx;

						TextCharacterInformation info;

						info.position.x = 0;
						info.position.y = scale * (bounds.b);
						// info.position.y = scale * (metrics.lineHeight - bounds.t);
						info.size = scale * size;

						info.textureIndex = hotTextureIndex;
						info.texturePosition = atlasScale * Vector2(xOffsetPx, yOffsetPx);
						info.textureSize = atlasScale * subbytesSize;

						xOffsetPx += subbytes.width() + marginPx;
						yOffsetMaxPx = Math::Max(yOffsetMaxPx, subbytes.height());

						characterInformationMap[character] = info;
					}

					Texture2D* texture = new Texture2D();
					texture->Read(hotBytes, atlasPx, atlasPx, channels, SamplerFilter::Linear);
					textures.push_back(texture);
				}
			}
		}
	};

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

				float Median(float r, float g, float b) 
				{
					return max(min(r, g), min(max(r, g), b));
				}


				void main() 
				{
					int textureIndex = int(vTextureIndex);
					vec4 sample = texture(uTextures[textureIndex], vTexturePosition);
					float distance = Median(sample.r, sample.g, sample.b);
					float screenPxDistance = vScale * (distance - 0.5);
					float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

					fColor = vColor;
					fColor.w *= opacity;

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

		void AddLine(const Vector2& position, const std::string& content, TextFont* font, float size, float thickness, float sharpness, const Vector4& color)
		{
			Vector2 offset(size * -font->padding * 0.5, size * -font->padding * 0.5);
			float scale = size / font->characterPx * font->distanceRange;

			for (usize i = 0; i < content.size(); ++i)
			{
				char character = content[i];

				if (character == ' ' || character == '\n') 
				{
					offset.x += size * 0.25;
					continue;
				}

				TextCharacterInformation& info = font->characterInformationMap[character];

				Vector2 localPosition = size * info.position;
				Vector2 localSize = size * info.size;

				Texture2D* texture = font->textures[info.textureIndex];

				AddCharacter(position + localPosition + offset, localSize, thickness, sharpness, scale, color, texture, info.texturePosition, info.textureSize);

				offset.x += localSize.x - (size * font->padding) + (size * font->GetKerning(character, content[i + 1]));
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


