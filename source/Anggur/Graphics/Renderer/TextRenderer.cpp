#include "Anggur/Graphics/Renderer/TextRenderer.h"
#include "Anggur/Graphics/API.h"

namespace Anggur
{
	TextRenderer::TextRenderer()
	{
		_vertices.assign(_batchVertex, TextVertex{});

		_indices.assign(_batchVertex * _batchIndexMultiplier, 0);

		_textures.assign(TextureSpecification::GetMaxSlot(), nullptr);
		_textureIndices.reserve(TextureSpecification::GetMaxSlot());

		for (size_t i = 0; i < TextureSpecification::GetMaxSlot(); ++i)
		{
			_textureIndices.push_back(i);
		}

		_vertexArray.Bind();
		_vertexArray.SetLayout({
			{VertexDataType::Float, 2, "aPosition"},
			{VertexDataType::Float, 2, "aTexturePosition"},
			{VertexDataType::Float, 1, "aTextureIndex"},
			{VertexDataType::Float, 1, "aThickness"},
			{VertexDataType::Float, 1, "aSharpness"},
			{VertexDataType::Float, 1, "aScale"},
			{VertexDataType::Float, 4, "aColor"},
		});

		_vertexBuffer.Bind();
		_vertexBuffer.SetCapacity(_vertexArray.GetStride() * _vertices.size());

		_indexBuffer.Bind();
		_indexBuffer.SetCapacity(sizeof(unsigned int) * _indices.size());

		_shader.Bind();
		_shader.SetVertexSource(R"(
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

		_shader.SetFragmentSource(
			R"(
			in vec2 vTexturePosition;
			in float vTextureIndex;
			in float vThickness;
			in float vSharpness;
			in float vScale;
			in vec4 vColor;

			out vec4 fColor;

			uniform sampler2D uTextures[TEXTURE_SLOT];

			void main() 
			{
				int textureIndex = int(vTextureIndex);

				vec4 color;

				TEXTURE_SLOT_INDEXING(color, uTextures, textureIndex, vTexturePosition)

				float thickness = 0.5f;

				fColor = vColor;
				float aa = fwidth(color.r);
				fColor.w *= smoothstep(thickness - aa, thickness + aa, color.r);
				// fColor.w *= color.r;

				if (fColor.w == 0.0f) 
				{
					discard;
				}
			}
		)"
		);
		_shader.Compile();
	}

	void TextRenderer::SetView(const Matrix3& newView)
	{
		_view = newView;
	}

	void TextRenderer::Add(
		const Vector2& position, unsigned int codepoint, Font* font, float size, float thickness, float sharpness,
		const Vector4& color
	)
	{
		if (!font)
			return;

		float padding = 1.0f / font->_sampleSize * font->_samplePadding * size;

		Vector2 pointer(-padding, -padding);

		if (codepoint == ' ' || codepoint == '\n')
		{
			return;
		}

		if (font->_glyphMap.count(codepoint) == 0)
		{
			if (!font->Generate(codepoint))
			{
				codepoint = 0xFFFD; // RC
			}
		}

		FontGlyph& glyph = font->_glyphMap[codepoint];

		Vector2 localPosition = size * glyph.position;
		Vector2 localSize = size * glyph.size;

		// sharpness = 1.0f / font->sampleSize * size * 0.1;

		AddCharacter(
			pointer + position + localPosition, localSize, thickness, sharpness, 1, color,
			font->_textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize
		);
	}

	void TextRenderer::Add(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		const Vector4& color
	)
	{
		if (!font)
			return;

		float padding = 1.0f / font->_sampleSize * font->_samplePadding * size;

		Vector2 pointer(-padding, -padding);

		bool startOfLine = true;

		for (size_t i = 0; i < content.size();)
		{
			unsigned int codepoint = TextEncoding::UTF8::CollapseAndMoveIndex(content, i);

			if (codepoint == ' ')
			{
				pointer.x += size * font->GetSpaceWidth();
				continue;
			}

			if (codepoint == '\n')
			{
				startOfLine = true;
				pointer.x = -padding;
				pointer.y += size * (font->GetLineHeight() + font->GetLineGap());
				continue;
			}

			if (font->_glyphMap.count(codepoint) == 0)
			{
				if (!font->Generate(codepoint))
				{
					codepoint = 0xFFFD; // RC
				}
			}

			FontGlyph& glyph = font->_glyphMap[codepoint];

			Vector2 localPosition = size * glyph.position;
			Vector2 localSize = size * glyph.size;

			if (startOfLine)
			{
				startOfLine = false;
				localPosition.x = 0;
			}

			AddCharacter(
				position + localPosition + pointer, localSize, thickness, sharpness, 1, color,
				font->_textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize
			);

			pointer.x += localPosition.x + localSize.x - (padding * 2) +
						 (size * font->GetKerning(codepoint, TextEncoding::UTF8::Collapse(content, i)));
		}
	}

	Vector2 TextRenderer::Measure(const std::string& content, Font* font, float size, float thickness, float sharpness)
	{
		if (!font)
			return Vector2(0, 0);

		float padding = 1.0f / font->_sampleSize * font->_samplePadding * size;

		Vector2 pointer(0, 0);

		float maxX = 0;

		bool startOfLine = true;

		for (size_t i = 0; i < content.size();)
		{
			unsigned int codepoint = TextEncoding::UTF8::CollapseAndMoveIndex(content, i);

			if (codepoint == ' ')
			{
				pointer.x += size * font->GetSpaceWidth();
				continue;
			}

			if (codepoint == '\n')
			{
				startOfLine = true;
				maxX = Math::Max(pointer.x, maxX);
				pointer.x = 0;
				continue;
			}

			if (font->_glyphMap.count(codepoint) == 0)
			{
				if (!font->Generate(codepoint))
				{
					codepoint = 0xFFFD; // RC
				}
			}

			FontGlyph& glyph = font->_glyphMap[codepoint];

			Vector2 localPosition = size * glyph.position;
			Vector2 localSize = size * glyph.size;

			if (startOfLine)
			{
				startOfLine = false;
				localPosition.x = 0;
				pointer.y += size * (font->GetLineHeight() + font->GetLineGap());
			}

			pointer.x += localPosition.x + localSize.x - (padding * 2) +
						 (size * font->GetKerning(codepoint, TextEncoding::UTF8::Collapse(content, i)));
		}

		pointer.x = Math::Max(pointer.x, maxX);

		return pointer;
	}

	void TextRenderer::AddLine(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		const Vector4& color
	)
	{
		if (!font)
			return;

		float padding = 1.0f / font->_sampleSize * font->_samplePadding * size;

		Vector2 pointer(-padding, -padding);

		bool startOfLine = true;

		for (size_t index = 0; index < content.size();)
		{
			unsigned int codepoint = TextEncoding::UTF8::CollapseAndMoveIndex(content, index);

			if (codepoint == ' ' || codepoint == '\t' || codepoint == '\n')
			{
				pointer.x += size * font->GetSpaceWidth();
				continue;
			}

			if (font->_glyphMap.count(codepoint) == 0)
			{
				if (!font->Generate(codepoint))
				{
					codepoint = 0xFFFD; // RC
				}
			}

			FontGlyph& glyph = font->_glyphMap[codepoint];

			Vector2 localPosition = size * glyph.position;
			Vector2 localSize = size * glyph.size;

			if (startOfLine)
			{
				startOfLine = false;
				localPosition.x = 0;
			}

			AddCharacter(
				pointer + position + localPosition, localSize, thickness, sharpness, 1, color,
				font->_textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize
			);

			pointer.x += localPosition.x + localSize.x - (padding * 2) +
						 (size * font->GetKerning(codepoint, TextEncoding::UTF8::Collapse(content, index)));
		}
	}

	void TextRenderer::AddLineCut(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		float limit, const Vector4& color
	)
	{
		AddPartialLineCut(position, content, 0, font, size, thickness, sharpness, limit, color);
	}

	void TextRenderer::AddPartialLineCut(
		const Vector2& position, const std::string& content, size_t contentOffset, Font* font, float size,
		float thickness, float sharpness, float limit, const Vector4& color
	)
	{
		if (!font)
			return;

		FontGlyph ellipsisGlyph = font->_glyphMap[0x2026];
		Vector2 ellipsisLocalPosition = size * ellipsisGlyph.position;
		Vector2 ellipsisLocalSize = size * ellipsisGlyph.size;

		float padding = 1.0f / font->_sampleSize * font->_samplePadding * size;

		Vector2 pointer(-padding, -padding);

		bool startOfLine = true;

		for (size_t index = contentOffset; index < content.size();)
		{
			unsigned int codepoint = TextEncoding::UTF8::CollapseAndMoveIndex(content, index);

			if (codepoint == ' ' || codepoint == '\t' || codepoint == '\n')
			{
				pointer.x += size * font->GetSpaceWidth();
				continue;
			}

			if (font->_glyphMap.count(codepoint) == 0)
			{
				if (!font->Generate(codepoint))
				{
					codepoint = 0xFFFD; // RC
				}
			}

			FontGlyph& glyph = font->_glyphMap[codepoint];

			Vector2 localPosition = size * glyph.position;
			Vector2 localSize = size * glyph.size;

			float ellipsisKerning = size * font->GetKerning(codepoint, 0x2026);

			if (pointer.x + localPosition.x + localSize.x + ellipsisLocalPosition.x + ellipsisLocalSize.x +
					ellipsisKerning >
				limit)
			{
				// AddCharacter(pointer + position + localPosition, localSize , thickness, sharpness, 1, color,
				// font->textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize);
				//
				// pointer.x += localPosition.x + localSize.x - (padding * 2) + ellipsisKerning;

				AddCharacter(
					pointer + position + ellipsisLocalPosition, ellipsisLocalSize, thickness, sharpness, 1, color,
					font->_textures[ellipsisGlyph.textureIndex], ellipsisGlyph.texturePosition, ellipsisGlyph.textureSize
				);
				break;
			}

			if (startOfLine)
			{
				startOfLine = false;
				localPosition.x = 0;
			}

			AddCharacter(
				pointer + position + localPosition, localSize, thickness, sharpness, 1, color,
				font->_textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize
			);

			pointer.x += localPosition.x + localSize.x - (padding * 2) +
						 (size * font->GetKerning(codepoint, TextEncoding::UTF8::Collapse(content, index)));
		}
	}

	void TextRenderer::AddFlow(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		float limit, const Vector4& color
	)
	{
		if (!font)
			return;

		float padding = 1.0f / font->_sampleSize * font->_samplePadding * size;

		Vector2 pointer(-padding, -padding);

		bool startOfLine = true;

		for (size_t i = 0; i < content.size();)
		{
			unsigned int codepoint = TextEncoding::UTF8::CollapseAndMoveIndex(content, i);

			if (codepoint == ' ')
			{
				pointer.x += size * font->GetSpaceWidth();
				continue;
			}

			if (codepoint == '\n')
			{
				startOfLine = true;
				pointer.x = -padding;
				pointer.y += size * (font->GetLineHeight() + font->GetLineGap());
				continue;
			}

			if (font->_glyphMap.count(codepoint) == 0)
			{
				if (!font->Generate(codepoint))
				{
					codepoint = 0xFFFD; // RC
				}
			}

			FontGlyph& glyph = font->_glyphMap[codepoint];

			Vector2 localPosition = size * glyph.position;
			Vector2 localSize = size * glyph.size;

			if (pointer.x + localPosition.x + localSize.x > limit)
			{
				startOfLine = true;
				pointer.x = -padding;
				pointer.y += size * (font->GetLineHeight() + font->GetLineGap());
			}

			if (startOfLine)
			{
				startOfLine = false;
				localPosition.x = 0;
			}

			AddCharacter(
				pointer + position + localPosition, localSize, thickness, sharpness, 1, color,
				font->_textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize
			);

			pointer.x += localPosition.x + localSize.x - (padding * 2) +
						 (size * font->GetKerning(codepoint, TextEncoding::UTF8::Collapse(content, i)));
		}
	}

	Vector2 TextRenderer::MeasureFlow(
		const std::string& content, Font* font, float size, float thickness, float sharpness, float limit
	)
	{
		if (!font)
			return Vector2(0, 0);

		float padding = 1.0f / font->_sampleSize * font->_samplePadding * size;

		Vector2 pointer(0, 0);

		bool startOfLine = true;

		for (size_t i = 0; i < content.size();)
		{
			unsigned int codepoint = TextEncoding::UTF8::CollapseAndMoveIndex(content, i);

			if (codepoint == ' ')
			{
				pointer.x += size * font->GetSpaceWidth();
				continue;
			}

			if (codepoint == '\n')
			{
				startOfLine = true;
				pointer.x = 0;
				pointer.y += size * (font->GetLineHeight() + font->GetLineGap());
				continue;
			}

			if (font->_glyphMap.count(codepoint) == 0)
			{
				if (!font->Generate(codepoint))
				{
					codepoint = 0xFFFD; // RC
				}
			}

			FontGlyph& glyph = font->_glyphMap[codepoint];

			Vector2 localPosition = size * glyph.position;
			Vector2 localSize = size * glyph.size;

			if (pointer.x + localPosition.x + localSize.x > limit)
			{
				startOfLine = true;
				pointer.x = 0;
				pointer.y += size * (font->GetLineHeight() + font->GetLineGap());
			}

			if (startOfLine)
			{
				startOfLine = false;
				localPosition.x = 0;
			}

			pointer.x += localPosition.x + localSize.x - (padding * 2) +
						 (size * font->GetKerning(codepoint, TextEncoding::UTF8::Collapse(content, i)));
		}

		pointer.y += size * (font->GetLineHeight() + font->GetLineGap());

		return pointer;
	}

	void TextRenderer::AddFlowCut(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		const Vector2& limit, const Vector4& color
	)
	{
		if (!font)
			return;

		float padding = 1.0f / font->_sampleSize * font->_samplePadding * size;

		Vector2 pointer(-padding, -padding);

		bool startOfLine = true;

		for (size_t i = 0; i < content.size();)
		{
			unsigned int codepoint = TextEncoding::UTF8::CollapseAndMoveIndex(content, i);

			if (codepoint == ' ')
			{
				pointer.x += size * font->GetSpaceWidth();
				continue;
			}

			if (codepoint == '\n')
			{
				startOfLine = true;
				pointer.x = -padding;
				pointer.y += size * (font->GetLineHeight() + font->GetLineGap());
				continue;
			}

			if (font->_glyphMap.count(codepoint) == 0)
			{
				if (!font->Generate(codepoint))
				{
					codepoint = 0xFFFD; // RC
				}
			}

			FontGlyph& glyph = font->_glyphMap[codepoint];

			Vector2 localPosition = size * glyph.position;
			Vector2 localSize = size * glyph.size;

			if (pointer.x + localPosition.x + localSize.x > limit.x)
			{
				startOfLine = true;
				pointer.x = -padding;
				pointer.y += size * (font->GetLineHeight() + font->GetLineGap());
			}

			if (startOfLine)
			{
				float lineHeight = size * (font->GetLineHeight() + font->GetLineGap());

				if (pointer.y + (2.0f * lineHeight) > limit.y)
				{
					AddPartialLineCut(
						position + pointer + padding, content, i, font, size, thickness, sharpness, limit.x, color
					);
					break;
				}

				startOfLine = false;
				localPosition.x = 0;
			}

			AddCharacter(
				pointer + position + localPosition, localSize, thickness, sharpness, 1, color,
				font->_textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize
			);

			pointer.x += localPosition.x + localSize.x - (padding * 2) +
						 (size * font->GetKerning(codepoint, TextEncoding::UTF8::Collapse(content, i)));
		}
	}

	void TextRenderer::AddCharacter(
		const Vector2& position, const Vector2& size, float thickness, float sharpness, float scale,
		const Vector4& color, Texture* texture, const Vector2& texturePosition, const Vector2& textureSize
	)
	{
		if (_vertexOffset + 4 > _vertices.size() || _indexOffset + 6 > _indices.size() ||
			_textureOffset + 1 > _textures.size())
		{
			Flush();
		}

		_vertices[_vertexOffset + 0].position.Set(position.x, position.y);
		_vertices[_vertexOffset + 1].position.Set(position.x + size.x, position.y);
		_vertices[_vertexOffset + 2].position.Set(position.x + size.x, position.y + size.y);
		_vertices[_vertexOffset + 3].position.Set(position.x, position.y + size.y);

		_vertices[_vertexOffset + 0].thickness = thickness;
		_vertices[_vertexOffset + 1].thickness = thickness;
		_vertices[_vertexOffset + 2].thickness = thickness;
		_vertices[_vertexOffset + 3].thickness = thickness;

		_vertices[_vertexOffset + 0].sharpness = sharpness;
		_vertices[_vertexOffset + 1].sharpness = sharpness;
		_vertices[_vertexOffset + 2].sharpness = sharpness;
		_vertices[_vertexOffset + 3].sharpness = sharpness;

		_vertices[_vertexOffset + 0].scale = scale;
		_vertices[_vertexOffset + 1].scale = scale;
		_vertices[_vertexOffset + 2].scale = scale;
		_vertices[_vertexOffset + 3].scale = scale;

		_vertices[_vertexOffset + 0].color = color;
		_vertices[_vertexOffset + 1].color = color;
		_vertices[_vertexOffset + 2].color = color;
		_vertices[_vertexOffset + 3].color = color;

		size_t textureIndex = 0;

		if (_textureIndexMap.count(texture->GetID()))
		{
			textureIndex = _textureIndexMap[texture->GetID()];
		}
		else
		{
			textureIndex = _textureOffset;
			_textureIndexMap[texture->GetID()] = textureIndex;
			_textures[textureIndex] = texture;

			++_textureOffset;
		}

		_vertices[_vertexOffset + 0].textureIndex = textureIndex;
		_vertices[_vertexOffset + 1].textureIndex = textureIndex;
		_vertices[_vertexOffset + 2].textureIndex = textureIndex;
		_vertices[_vertexOffset + 3].textureIndex = textureIndex;

		_vertices[_vertexOffset + 0].texturePosition.Set(texturePosition.x, texturePosition.y);
		_vertices[_vertexOffset + 1].texturePosition.Set(texturePosition.x + textureSize.x, texturePosition.y);
		_vertices[_vertexOffset + 2].texturePosition.Set(
			texturePosition.x + textureSize.x, texturePosition.y + textureSize.y
		);
		_vertices[_vertexOffset + 3].texturePosition.Set(texturePosition.x, texturePosition.y + textureSize.y);

		_indices[_indexOffset + 0] = _vertexOffset + 0;
		_indices[_indexOffset + 1] = _vertexOffset + 1;
		_indices[_indexOffset + 2] = _vertexOffset + 2;
		_indices[_indexOffset + 3] = _vertexOffset + 2;
		_indices[_indexOffset + 4] = _vertexOffset + 3;
		_indices[_indexOffset + 5] = _vertexOffset + 0;

		_vertexOffset += 4;

		_indexOffset += 6;
	}

	void TextRenderer::Flush()
	{
		// Early exit if no vertices to draw
		if (_vertexOffset == 0)
			return;

		for (size_t textureIndex = 0; textureIndex < _textureOffset; ++textureIndex)
			_textures[textureIndex]->Bind(textureIndex);

		_shader.Bind();
		_shader.SetUniformMatrix3("uView", _view);
		_shader.SetUniformInt("uTextures", _textureOffset, _textureIndices.data());

		_vertexArray.Bind();

		_vertexBuffer.Bind();
		_vertexBuffer.SetData(_vertexArray.GetStride() * _vertexOffset, _vertices.data());

		_indexBuffer.Bind();
		_indexBuffer.SetData(sizeof(unsigned int) * _indexOffset, _indices.data());

		glDrawElements(GL_TRIANGLES, _indexOffset, GL_UNSIGNED_INT, nullptr);

		_vertexOffset = 0;
		_indexOffset = 0;
		_textureOffset = 0;

		_textureIndexMap.clear();
	}
}
