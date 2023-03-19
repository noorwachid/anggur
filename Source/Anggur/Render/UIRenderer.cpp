#include "Anggur/Graphics/Function.h"
#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Render/UIRenderer.h"
#include <iostream>
#include <vector>

namespace Anggur
{
	UIRenderer::UIRenderer()
	{
		InitializeVertexPool();
		InitializeTexturePool();
		initializeShaders();
	}

	UIRenderer::~UIRenderer()
	{
	}

	void UIRenderer::InitializeVertexPool()
	{
		vertices.assign(
			batchVertex, UIVertex(Vector2(0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f)
		);
		indices.assign(batchVertex * batchIndexMultiplier, 0);

		vertexBuffer.SetCapacity(sizeof(UIVertex) * vertices.size());

		vertexArray.SetAttribute(0, 2, GL_FLOAT, sizeof(UIVertex), offsetof(UIVertex, position));
		vertexArray.SetAttribute(1, 4, GL_FLOAT, sizeof(UIVertex), offsetof(UIVertex, color));
		vertexArray.SetAttribute(2, 2, GL_FLOAT, sizeof(UIVertex), offsetof(UIVertex, uv));
		vertexArray.SetAttribute(3, 1, GL_FLOAT, sizeof(UIVertex), offsetof(UIVertex, slot));

		indexBuffer.SetCapacity(sizeof(uint32_t) * indices.size());
	}

	void UIRenderer::InitializeTexturePool()
	{
		textures.assign(TextureSpecification::GetMaxSlot(), nullptr);
		slots.reserve(TextureSpecification::GetMaxSlot());

		for (int i = 0; i < TextureSpecification::GetMaxSlot(); ++i)
		{
			slots.push_back(i);
		}
		std::vector<uint8_t> bytes = {{255, 255, 255, 255}};
		whiteTexture = std::make_shared<Texture2D>(bytes, 1, 1, 4);
	}

	void UIRenderer::initializeShaders()
	{
		textShader.Bind();
		textShader.SetVertexSource(R"(
            #version 330 core

            layout (location = 0) in vec2 aPosition;
            layout (location = 1) in vec4 aColor;
            layout (location = 2) in vec2 aUV;
            layout (location = 3) in float aSlot;

            out vec4 vColor;
            out vec2 vUV;
            out float vSlot;

            uniform mat3 uViewProjection;

            void main() {
                gl_Position = vec4(uViewProjection * vec3(aPosition, 1.0f), 1.0f);

                vColor = aColor;
                vUV = aUV;
                vSlot = aSlot;
            }
        )");
		textShader.SetFragmentSource(
			R"(
            #version 330 core
            
            in vec4 vColor;
            in vec2 vUV;
            in float vSlot;

            out vec4 fColor;

            uniform sampler2D uSlots[)" +
			std::to_string(TextureSpecification::GetMaxSlot()) + R"(];
            
            void main() {
                float distance = texture(uSlots[int(vSlot)], vUV).r;
                float lowThreshold = 0.8f;
                float highThreshold = 0.9f;
                fColor.rgb = vColor.rgb;

                if (distance > highThreshold) {
                    fColor.a = 1.0f;
                }
                else if (distance > lowThreshold) {
                    fColor.a = smoothstep(0.0f, 1.0f, distance);
                }
                else {
                    fColor.a = 0.0f;
                }
            }
        )"
		);
		textShader.Compile();

		geometryShader.Bind();
		geometryShader.SetVertexSource(R"(
            #version 330 core

            layout (location = 0) in vec2 aPosition;
            layout (location = 1) in vec4 aColor;
            layout (location = 2) in vec2 aUV;
            layout (location = 3) in float aSlot;

            out vec4 vColor;
            out vec2 vUV;
            out float vSlot;

            uniform mat3 uViewProjection;

            void main() {
                gl_Position = vec4(uViewProjection * vec3(aPosition, 1.0f), 1.0f);

                vColor = aColor;
                vUV = aUV;
                vSlot = aSlot;
            }
        )");
		geometryShader.SetFragmentSource(
			R"(
            #version 330 core
            
            in vec4 vColor;
            in vec2 vUV;
            in float vSlot;

            out vec4 fColor;

            uniform sampler2D uSlots[)" +
			std::to_string(TextureSpecification::GetMaxSlot()) +
			R"(];
            
            void main() {
                fColor = texture(uSlots[int(vSlot)], vUV) * vColor;
            }
        )"
		);
		geometryShader.Compile();
	}

	void UIRenderer::SwitchDrawingMode(DrawingMode newMode)
	{
		if (drawingMode != newMode)
		{
			Flush();
			drawingMode = newMode;
		}
	}

	void UIRenderer::SetBatchChunk(size_t vertex, size_t indexMultiplier)
	{
		batchVertex = vertex;
		batchIndexMultiplier = indexMultiplier;
	}

	void UIRenderer::Clear(const Vector4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void UIRenderer::SetViewport(const Vector2& size)
	{
		glViewport(0, 0, size.x, size.y);
	}

	void UIRenderer::SetViewProjection(const Matrix3& newViewProjection)
	{
		viewProjection = newViewProjection;
	}

	void UIRenderer::BeginScene()
	{
		drawCount = 0;

		FlushInternalBuffer();
	}

	void UIRenderer::BeginScene(const Matrix3& viewProjection)
	{
		SetViewProjection(viewProjection);
		BeginScene();
	}

	void UIRenderer::EndScene()
	{
		Flush();
	}

	void UIRenderer::Flush()
	{
		if (vertexOffset == 0)
		{
			return;
		}

		for (size_t i = 0; i < textureOffset; ++i)
		{
			textures[i]->Bind(i);
		}

		switch (drawingMode)
		{
			case DrawingMode::Text:
				textShader.Bind();
				textShader.SetUniformMatrix3("uViewProjection", viewProjection);
				textShader.SetUniformInt("uSlots", textureOffset, slots.data());
				break;

			case DrawingMode::Geometry:
			default:
				geometryShader.Bind();
				geometryShader.SetUniformMatrix3("uViewProjection", viewProjection);
				geometryShader.SetUniformInt("uSlots", textureOffset, slots.data());
				break;
		}

		vertexArray.Bind();

		vertexBuffer.Bind();
		vertexBuffer.setData(sizeof(UIVertex) * vertexOffset, vertices.data());

		indexBuffer.Bind();
		indexBuffer.setData(sizeof(uint32_t) * indexOffset, indices.data());

		glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

		FlushInternalBuffer();

		++drawCount;
	}

	void UIRenderer::FlushInternalBuffer()
	{
		vertexOffset = 0;
		indexOffset = 0;
		textureOffset = 0;
	}

	bool UIRenderer::IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize)
	{
		return vertexOffset + newVertexSize > vertices.size() || indexOffset + newIndexSize > indices.size() ||
			   textureOffset + newTextureSize > textures.size();
	}

	void UIRenderer::Draw(
		const std::vector<UIVertex>& newVertices, 
		const std::vector<uint32_t>& newIndices,
		const std::shared_ptr<Texture2D>& texture
	)
	{
		if (IsCapacityMaxout(newVertices.size(), newIndices.size(), 1))
			Flush();

		// Find or add new texture slot
		int textureSlot = 0;

		// This code only create one branch
		for (; textureSlot < textureOffset && textures[textureSlot]->GetID() != texture->GetID(); ++textureSlot)
			;

		if (textureSlot == textureOffset)
		{
			textureSlot = textureOffset;
			textures[textureOffset] = texture;
			textureOffset += 1;
		}

		for (size_t i = 0; i < newVertices.size(); ++i)
		{
			auto& vertex = vertices[i + vertexOffset];

			vertex = newVertices[i];
			vertex.slot = textureSlot;
		}

		for (size_t i = 0; i < newIndices.size(); ++i)
		{
			indices[i + indexOffset] = newIndices[i] + vertexOffset;
		}

		vertexOffset += newVertices.size();
		indexOffset += newIndices.size();
	}

	void UIRenderer::DrawRectangle(const Vector2& position, const Vector2& size, const Vector4& color)
	{
		SwitchDrawingMode(DrawingMode::Geometry);

		Draw(
			{
				UIVertex(position,                                          color, Vector2(0, 0)),
				UIVertex(Vector2(position.x + size.x, position.y),          color, Vector2(0, 0)),
				UIVertex(Vector2(position.x + size.x, position.y + size.y), color, Vector2(0, 0)),
				UIVertex(Vector2(position.x, position.y + size.y),          color, Vector2(0, 0)),
			},
			{
				0, 1, 2,
				2, 3, 0,
			},
			whiteTexture
		);
	}

	void UIRenderer::DrawText(
		const Vector2& position, 
		const Vector2& size, 
		const std::string& text, 
		const std::shared_ptr<Font> font,
		float fontSize,
		float letterSpacing,
		float wordSpacing,
		float lineSpacing
	)
	{
		SwitchDrawingMode(DrawingMode::Text);

		Vector2 pointer = fontSize * Vector2(-font->glyphPadding, -font->glyphPadding);

		for (usize i = 0; i < text.size(); ++i)
		{
			char codePoint = text[i];

			if (codePoint == ' ')
			{
				pointer.x += wordSpacing;
				continue;
			}
			else
			{
				pointer.x += letterSpacing;
			}

			Glyph glyph = font->glyphMap[codePoint];
			DrawTextGlyph(
				Vector2(pointer.x, pointer.y + (glyph.ascent * fontSize)), 
				fontSize * glyph.size ,
				font->glyphBuffers[glyph.bufferIndex].texture, 
				glyph.texturePosition, glyph.textureSize
			);

			pointer.x += (glyph.size.x - 2 * font->glyphPadding + font->GetKerning(codePoint, text[i + 1])) * fontSize;
		}
	}

	void UIRenderer::DrawTextGlyph(
		const Vector2& position, 
		const Vector2& size, 
		const std::shared_ptr<Texture2D>& texture,
		const Vector2& texturePosition, 
		const Vector2& textureSize, 
		const Vector4& color
	)
	{
		SwitchDrawingMode(DrawingMode::Text);

		Draw(
			{
				UIVertex(position,                                          color, texturePosition),
				UIVertex(Vector2(position.x + size.x, position.y),          color, Vector2(texturePosition.x + textureSize.x, texturePosition.y)),
				UIVertex(Vector2(position.x + size.x, position.y + size.y), color, Vector2(texturePosition.x + textureSize.x, texturePosition.y + textureSize.y)),
				UIVertex(Vector2(position.x, position.y + size.y),          color, Vector2(texturePosition.x, texturePosition.y + textureSize.y)),
			},
			{
				0, 1, 2,
				2, 3, 0,
			},
			texture
		);
	}
}

