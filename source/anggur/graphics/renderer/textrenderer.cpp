#include "anggur/graphics/renderer/textrenderer.h"
#include "anggur/graphics/api.h"

namespace Anggur {
	TextRenderer::TextRenderer() {
		vertices.assign(batchVertex, TextVertex{});

		indices.assign(batchVertex * batchIndexMultiplier, 0);

		textures.assign(TextureSpecification::getMaxSlot(), nullptr);
		textureIndices.reserve(TextureSpecification::getMaxSlot());

		for (size_t i = 0; i < TextureSpecification::getMaxSlot(); ++i) {
			textureIndices.push_back(i);
		}

		vertexArray.bind();
		vertexArray.setLayout({
			{VertexDataType::Float, 2, "aPosition"},
			{VertexDataType::Float, 2, "aTexturePosition"},
			{VertexDataType::Float, 1, "aTextureIndex"},
			{VertexDataType::Float, 1, "aThickness"},
			{VertexDataType::Float, 1, "aSharpness"},
			{VertexDataType::Float, 1, "aScale"},
			{VertexDataType::Float, 4, "aColor"},
		});

		vertexBuffer.bind();
		vertexBuffer.setCapacity(vertexArray.getStride() * vertices.size());

		indexBuffer.bind();
		indexBuffer.setCapacity(sizeof(unsigned int) * indices.size());

		shader.bind();
		shader.setVertexSource(R"(
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

		shader.setFragmentSource(
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
		shader.compile();
	}

	void TextRenderer::setView(const Matrix3& newView) {
		view = newView;
	}

	void TextRenderer::add(
		const Vector2& position, unsigned int codepoint, Font* font, float size, float thickness, float sharpness,
		const Vector4& color
	) {
		if (!font)
			return;

		float padding = 1.0f / font->sampleSize * font->samplePadding * size;

		Vector2 pointer(-padding, -padding);

		if (codepoint == ' ' || codepoint == '\n') {
			return;
		}

		if (font->glyphMap.count(codepoint) == 0) {
			if (!font->generate(codepoint)) {
				codepoint = 0xFFFD; // RC
			}
		}

		FontGlyph& glyph = font->glyphMap[codepoint];

		Vector2 localPosition = size * glyph.position;
		Vector2 localSize = size * glyph.size;

		// sharpness = 1.0f / font->sampleSize * size * 0.1;

		addCharacter(
			pointer + position + localPosition, localSize, thickness, sharpness, 1, color,
			font->textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize
		);
	}

	void TextRenderer::add(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		const Vector4& color
	) {
		if (!font)
			return;

		float padding = 1.0f / font->sampleSize * font->samplePadding * size;

		Vector2 pointer(-padding, -padding);

		bool startOfLine = true;

		for (size_t i = 0; i < content.size();) {
			unsigned int codepoint = TextEncoding::UTF8::collapseAndMoveIndex(content, i);

			if (codepoint == ' ') {
				pointer.x += size * font->getSpaceWidth();
				continue;
			}

			if (codepoint == '\n') {
				startOfLine = true;
				pointer.x = -padding;
				pointer.y += size * (font->getLineHeight() + font->getLineGap());
				continue;
			}

			if (font->glyphMap.count(codepoint) == 0) {
				if (!font->generate(codepoint)) {
					codepoint = 0xFFFD; // RC
				}
			}

			FontGlyph& glyph = font->glyphMap[codepoint];

			Vector2 localPosition = size * glyph.position;
			Vector2 localSize = size * glyph.size;

			if (startOfLine) {
				startOfLine = false;
				localPosition.x = 0;
			}

			addCharacter(
				position + localPosition + pointer, localSize, thickness, sharpness, 1, color,
				font->textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize
			);

			pointer.x += localPosition.x + localSize.x - (padding * 2) +
						 (size * font->getKerning(codepoint, TextEncoding::UTF8::collapse(content, i)));
		}
	}

	Vector2 TextRenderer::Measure(
		const std::string& content, Font* font, float size, float thickness, float sharpness
	) {
		if (!font)
			return Vector2(0, 0);

		float padding = 1.0f / font->sampleSize * font->samplePadding * size;

		Vector2 pointer(0, 0);

		float maxX = 0;

		bool startOfLine = true;

		for (size_t i = 0; i < content.size();) {
			unsigned int codepoint = TextEncoding::UTF8::collapseAndMoveIndex(content, i);

			if (codepoint == ' ') {
				pointer.x += size * font->getSpaceWidth();
				continue;
			}

			if (codepoint == '\n') {
				startOfLine = true;
				maxX = Math::max(pointer.x, maxX);
				pointer.x = 0;
				continue;
			}

			if (font->glyphMap.count(codepoint) == 0) {
				if (!font->generate(codepoint)) {
					codepoint = 0xFFFD; // RC
				}
			}

			FontGlyph& glyph = font->glyphMap[codepoint];

			Vector2 localPosition = size * glyph.position;
			Vector2 localSize = size * glyph.size;

			if (startOfLine) {
				startOfLine = false;
				localPosition.x = 0;
				pointer.y += size * (font->getLineHeight() + font->getLineGap());
			}

			pointer.x += localPosition.x + localSize.x - (padding * 2) +
						 (size * font->getKerning(codepoint, TextEncoding::UTF8::collapse(content, i)));
		}

		pointer.x = Math::max(pointer.x, maxX);

		return pointer;
	}

	void TextRenderer::addLine(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		const Vector4& color
	) {
		if (!font)
			return;

		float padding = 1.0f / font->sampleSize * font->samplePadding * size;

		Vector2 pointer(-padding, -padding);

		bool startOfLine = true;

		for (size_t index = 0; index < content.size();) {
			unsigned int codepoint = TextEncoding::UTF8::collapseAndMoveIndex(content, index);

			if (codepoint == ' ' || codepoint == '\t' || codepoint == '\n') {
				pointer.x += size * font->getSpaceWidth();
				continue;
			}

			if (font->glyphMap.count(codepoint) == 0) {
				if (!font->generate(codepoint)) {
					codepoint = 0xFFFD; // RC
				}
			}

			FontGlyph& glyph = font->glyphMap[codepoint];

			Vector2 localPosition = size * glyph.position;
			Vector2 localSize = size * glyph.size;

			if (startOfLine) {
				startOfLine = false;
				localPosition.x = 0;
			}

			addCharacter(
				pointer + position + localPosition, localSize, thickness, sharpness, 1, color,
				font->textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize
			);

			pointer.x += localPosition.x + localSize.x - (padding * 2) +
						 (size * font->getKerning(codepoint, TextEncoding::UTF8::collapse(content, index)));
		}
	}

	void TextRenderer::addLineCut(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		float limit, const Vector4& color
	) {
		addPartialLineCut(position, content, 0, font, size, thickness, sharpness, limit, color);
	}

	void TextRenderer::addPartialLineCut(
		const Vector2& position, const std::string& content, size_t contentOffset, Font* font, float size,
		float thickness, float sharpness, float limit, const Vector4& color
	) {
		if (!font)
			return;

		FontGlyph ellipsisGlyph = font->glyphMap[0x2026];
		Vector2 ellipsisLocalPosition = size * ellipsisGlyph.position;
		Vector2 ellipsisLocalSize = size * ellipsisGlyph.size;

		float padding = 1.0f / font->sampleSize * font->samplePadding * size;

		Vector2 pointer(-padding, -padding);

		bool startOfLine = true;

		for (size_t index = contentOffset; index < content.size();) {
			unsigned int codepoint = TextEncoding::UTF8::collapseAndMoveIndex(content, index);

			if (codepoint == ' ' || codepoint == '\t' || codepoint == '\n') {
				pointer.x += size * font->getSpaceWidth();
				continue;
			}

			if (font->glyphMap.count(codepoint) == 0) {
				if (!font->generate(codepoint)) {
					codepoint = 0xFFFD; // RC
				}
			}

			FontGlyph& glyph = font->glyphMap[codepoint];

			Vector2 localPosition = size * glyph.position;
			Vector2 localSize = size * glyph.size;

			float ellipsisKerning = size * font->getKerning(codepoint, 0x2026);

			if (pointer.x + localPosition.x + localSize.x + ellipsisLocalPosition.x + ellipsisLocalSize.x +
					ellipsisKerning >
				limit) {
				// AddCharacter(pointer + position + localPosition, localSize , thickness, sharpness, 1, color,
				// font->textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize);
				//
				// pointer.x += localPosition.x + localSize.x - (padding * 2) + ellipsisKerning;

				addCharacter(
					pointer + position + ellipsisLocalPosition, ellipsisLocalSize, thickness, sharpness, 1, color,
					font->textures[ellipsisGlyph.textureIndex], ellipsisGlyph.texturePosition,
					ellipsisGlyph.textureSize
				);
				break;
			}

			if (startOfLine) {
				startOfLine = false;
				localPosition.x = 0;
			}

			addCharacter(
				pointer + position + localPosition, localSize, thickness, sharpness, 1, color,
				font->textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize
			);

			pointer.x += localPosition.x + localSize.x - (padding * 2) +
						 (size * font->getKerning(codepoint, TextEncoding::UTF8::collapse(content, index)));
		}
	}

	void TextRenderer::addFlow(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		float limit, const Vector4& color
	) {
		if (!font)
			return;

		float padding = 1.0f / font->sampleSize * font->samplePadding * size;

		Vector2 pointer(-padding, -padding);

		bool startOfLine = true;

		for (size_t i = 0; i < content.size();) {
			unsigned int codepoint = TextEncoding::UTF8::collapseAndMoveIndex(content, i);

			if (codepoint == ' ') {
				pointer.x += size * font->getSpaceWidth();
				continue;
			}

			if (codepoint == '\n') {
				startOfLine = true;
				pointer.x = -padding;
				pointer.y += size * (font->getLineHeight() + font->getLineGap());
				continue;
			}

			if (font->glyphMap.count(codepoint) == 0) {
				if (!font->generate(codepoint)) {
					codepoint = 0xFFFD; // RC
				}
			}

			FontGlyph& glyph = font->glyphMap[codepoint];

			Vector2 localPosition = size * glyph.position;
			Vector2 localSize = size * glyph.size;

			if (pointer.x + localPosition.x + localSize.x > limit) {
				startOfLine = true;
				pointer.x = -padding;
				pointer.y += size * (font->getLineHeight() + font->getLineGap());
			}

			if (startOfLine) {
				startOfLine = false;
				localPosition.x = 0;
			}

			addCharacter(
				pointer + position + localPosition, localSize, thickness, sharpness, 1, color,
				font->textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize
			);

			pointer.x += localPosition.x + localSize.x - (padding * 2) +
						 (size * font->getKerning(codepoint, TextEncoding::UTF8::collapse(content, i)));
		}
	}

	void TextRenderer::addFlowCut(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		const Vector2& limit, const Vector4& color
	) {
		if (!font)
			return;

		float padding = 1.0f / font->sampleSize * font->samplePadding * size;

		Vector2 pointer(-padding, -padding);

		bool startOfLine = true;

		for (size_t i = 0; i < content.size();) {
			unsigned int codepoint = TextEncoding::UTF8::collapseAndMoveIndex(content, i);

			if (codepoint == ' ') {
				pointer.x += size * font->getSpaceWidth();
				continue;
			}

			if (codepoint == '\n') {
				startOfLine = true;
				pointer.x = -padding;
				pointer.y += size * (font->getLineHeight() + font->getLineGap());
				continue;
			}

			if (font->glyphMap.count(codepoint) == 0) {
				if (!font->generate(codepoint)) {
					codepoint = 0xFFFD; // RC
				}
			}

			FontGlyph& glyph = font->glyphMap[codepoint];

			Vector2 localPosition = size * glyph.position;
			Vector2 localSize = size * glyph.size;

			if (pointer.x + localPosition.x + localSize.x > limit.x) {
				startOfLine = true;
				pointer.x = -padding;
				pointer.y += size * (font->getLineHeight() + font->getLineGap());
			}

			if (startOfLine) {
				float lineHeight = size * (font->getLineHeight() + font->getLineGap());

				if (pointer.y + (2.0f * lineHeight) > limit.y) {
					addPartialLineCut(
						position + pointer + padding, content, i, font, size, thickness, sharpness, limit.x, color
					);
					break;
				}

				startOfLine = false;
				localPosition.x = 0;
			}

			addCharacter(
				pointer + position + localPosition, localSize, thickness, sharpness, 1, color,
				font->textures[glyph.textureIndex], glyph.texturePosition, glyph.textureSize
			);

			pointer.x += localPosition.x + localSize.x - (padding * 2) +
						 (size * font->getKerning(codepoint, TextEncoding::UTF8::collapse(content, i)));
		}
	}

	void TextRenderer::addCharacter(
		const Vector2& position, const Vector2& size, float thickness, float sharpness, float scale,
		const Vector4& color, Texture* texture, const Vector2& texturePosition, const Vector2& textureSize
	) {
		if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > indices.size() ||
			textureOffset + 1 > textures.size()) {
			flush();
		}

		vertices[vertexOffset + 0].position.set(position.x, position.y);
		vertices[vertexOffset + 1].position.set(position.x + size.x, position.y);
		vertices[vertexOffset + 2].position.set(position.x + size.x, position.y + size.y);
		vertices[vertexOffset + 3].position.set(position.x, position.y + size.y);

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

		size_t textureIndex = 0;

		if (textureIndexMap.count(texture->getID())) {
			textureIndex = textureIndexMap[texture->getID()];
		} else {
			textureIndex = textureOffset;
			textureIndexMap[texture->getID()] = textureIndex;
			textures[textureIndex] = texture;

			++textureOffset;
		}

		vertices[vertexOffset + 0].textureIndex = textureIndex;
		vertices[vertexOffset + 1].textureIndex = textureIndex;
		vertices[vertexOffset + 2].textureIndex = textureIndex;
		vertices[vertexOffset + 3].textureIndex = textureIndex;

		vertices[vertexOffset + 0].texturePosition.set(texturePosition.x, texturePosition.y);
		vertices[vertexOffset + 1].texturePosition.set(texturePosition.x + textureSize.x, texturePosition.y);
		vertices[vertexOffset + 2].texturePosition.set(
			texturePosition.x + textureSize.x, texturePosition.y + textureSize.y
		);
		vertices[vertexOffset + 3].texturePosition.set(texturePosition.x, texturePosition.y + textureSize.y);

		indices[indexOffset + 0] = vertexOffset + 0;
		indices[indexOffset + 1] = vertexOffset + 1;
		indices[indexOffset + 2] = vertexOffset + 2;
		indices[indexOffset + 3] = vertexOffset + 2;
		indices[indexOffset + 4] = vertexOffset + 3;
		indices[indexOffset + 5] = vertexOffset + 0;

		vertexOffset += 4;

		indexOffset += 6;
	}

	void TextRenderer::flush() {
		// Early exit if no vertices to draw
		if (vertexOffset == 0)
			return;

		for (size_t textureIndex = 0; textureIndex < textureOffset; ++textureIndex)
			textures[textureIndex]->bind(textureIndex);

		shader.bind();
		shader.setUniformMatrix3("uView", view);
		shader.setUniformInt("uTextures", textureOffset, textureIndices.data());

		vertexArray.bind();

		vertexBuffer.bind();
		vertexBuffer.setData(vertexArray.getStride() * vertexOffset, vertices.data());

		indexBuffer.bind();
		indexBuffer.setData(sizeof(unsigned int) * indexOffset, indices.data());

		glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

		vertexOffset = 0;
		indexOffset = 0;
		textureOffset = 0;

		textureIndexMap.clear();
	}
}
