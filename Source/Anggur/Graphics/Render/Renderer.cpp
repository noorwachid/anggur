#include "Anggur/Graphics/Function.h"
#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Graphics/Render/Renderer.h"
#include "Anggur/Math/Vector2.h"
#include <iostream>
#include <vector>

namespace Anggur
{
	Renderer::Renderer()
	{
		InitializeVertexPool();
		InitializeTexturePool();
		InitializeShaders();
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::InitializeVertexPool()
	{
		vertices.assign(
			batchVertex, Vertex(Vector2(0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f)
		);
		indices.assign(batchVertex * batchIndexMultiplier, 0);

		vertexBuffer.SetCapacity(sizeof(Vertex) * vertices.size());

		vertexArray.SetAttribute(0, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
		vertexArray.SetAttribute(1, 4, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
		vertexArray.SetAttribute(2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, uv));
		vertexArray.SetAttribute(3, 1, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, slot));

		indexBuffer.SetCapacity(sizeof(uint) * indices.size());
	}

	void Renderer::InitializeTexturePool()
	{
		textures.assign(TextureSpecification::GetMaxSlot(), nullptr);
		slots.reserve(TextureSpecification::GetMaxSlot());

		for (int i = 0; i < TextureSpecification::GetMaxSlot(); ++i)
		{
			slots.push_back(i);
		}

		std::vector<uint8_t> bytes = {{255, 255, 255, 255}};
		whiteTexture = new Texture2D(bytes, 1, 1, 4);
	}

	void Renderer::InitializeShaders()
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
                float land = 0.8f;
                float shore = 0.05f;

                fColor.rgb = vColor.rgb;
                fColor.a = smoothstep(land - shore, land + shore, distance);
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

	void Renderer::SwitchDrawingMode(DrawingMode newMode)
	{
		if (drawingMode != newMode)
		{
			Flush();
			drawingMode = newMode;
		}
	}

	void Renderer::SetBatchChunk(usize vertex, usize indexMultiplier)
	{
		batchVertex = vertex;
		batchIndexMultiplier = indexMultiplier;
	}

	void Renderer::Clear(const Vector4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::SetViewport(const Vector2& position, const Vector2& size)
	{
		glViewport(position.x, position.y, size.x, size.y);
	}

	void Renderer::SetViewProjection(const Matrix3& newViewProjection)
	{
		viewProjection = newViewProjection;
	}

	void Renderer::BeginScene()
	{
		drawCount = 0;

		FlushInternalBuffer();
	}

	void Renderer::BeginScene(const Matrix3& viewProjection)
	{
		SetViewProjection(viewProjection);
		BeginScene();
	}

	void Renderer::EndScene()
	{
		Flush();
	}

	void Renderer::Flush()
	{
		if (vertexOffset == 0)
		{
			return;
		}

		for (usize i = 0; i < textureOffset; ++i)
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
		vertexBuffer.setData(sizeof(Vertex) * vertexOffset, vertices.data());

		indexBuffer.Bind();
		indexBuffer.setData(sizeof(uint) * indexOffset, indices.data());

		glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

		FlushInternalBuffer();

		++drawCount;
	}

	void Renderer::FlushInternalBuffer()
	{
		vertexOffset = 0;
		indexOffset = 0;
		textureOffset = 0;
	}

	bool Renderer::IsCapacityMaxout(usize newVertexSize, usize newIndexSize, usize newTextureSize)
	{
		return vertexOffset + newVertexSize > vertices.size() || indexOffset + newIndexSize > indices.size() ||
			   textureOffset + newTextureSize > textures.size();
	}

	void Renderer::Draw(
		const std::vector<Vertex>& newVertices, 
		const std::vector<uint>& newIndices,
		Texture2D* texture
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

		for (usize i = 0; i < newVertices.size(); ++i)
		{
			auto& vertex = vertices[i + vertexOffset];

			vertex = newVertices[i];
			vertex.slot = textureSlot;
		}

		for (usize i = 0; i < newIndices.size(); ++i)
		{
			indices[i + indexOffset] = newIndices[i] + vertexOffset;
		}

		vertexOffset += newVertices.size();
		indexOffset += newIndices.size();
	}

	// 2D primitives

	void Renderer::DrawTriangle(
		const Matrix3& model, 
		const Vector2& position0, 
		const Vector2& position1, 
		const Vector2& position2, 
		const Vector4& color
	)
	{
		SwitchDrawingMode(DrawingMode::Geometry);

		Draw(
			{
				Vertex(model * position0, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
				Vertex(model * position1, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
				Vertex(model * position2, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
			},
			{
				0, 1, 2,
			},
			whiteTexture
		);
	}

	void Renderer::DrawQuad(
		const Matrix3& model, 
		const Vector2& position0, 
		const Vector2& position1, 
		const Vector2& position2,
		const Vector2& position3, 
		const Vector4& color
	)
	{
		SwitchDrawingMode(DrawingMode::Geometry);

		Draw(
			{
				Vertex(model * position0, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
				Vertex(model * position1, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
				Vertex(model * position2, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
				Vertex(model * position3, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
			},
			{
				0, 1, 2, 2, 3, 0,
			},
			whiteTexture
		);
	}

	void Renderer::DrawRectangle(
		const Matrix3& model, 
		const Vector2& position, 
		const Vector2& size, 
		const Vector4& color
	)
	{
		SwitchDrawingMode(DrawingMode::Geometry);

		Draw(
			{
				Vertex(model * position, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
				Vertex(model * Vector2(position.x + size.x, position.y), Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
				Vertex(model * Vector2(position.x + size.x, position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
				Vertex(model * Vector2(position.x, position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
			},
			{
				0, 1, 2, 2, 3, 0,
			},
			whiteTexture
		);
	}

	void Renderer::DrawRectangle(
		const Matrix3& model, 
		const Vector2& position, 
		const Vector2& size, 
		Texture2D* texture,
		const Vector2& texturePosition, 
		const Vector2& textureSize, 
		const Vector4& color
	)
	{
		SwitchDrawingMode(DrawingMode::Geometry);

		Draw(
			{
				Vertex(model * position, Vector4(color.x, color.y, color.z, color.w), texturePosition),
				Vertex(model * Vector2(position.x + size.x, position.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x + textureSize.x, texturePosition.y)),
				Vertex(model * Vector2(position.x + size.x, position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x + textureSize.x, texturePosition.y + textureSize.y)),
				Vertex(model * Vector2(position.x, position.y + size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x, texturePosition.y + textureSize.y)),
			},
			{
				0, 1, 2, 2, 3, 0,
			},
			texture
		);
	}

	void Renderer::DrawCircle(const Matrix3& model, float radius, int segment, const Vector4& color)
	{
		SwitchDrawingMode(DrawingMode::Geometry);

		if (segment < 3)
			segment = 3;

		Vector2 position(0, 0);
		usize triangleSize = segment - 2;

		float theta = (Math::twoPi / segment);
		float tangetialFactor = Math::Tan(theta);
		float radialFactor = Math::Cos(theta);

		float x = radius;
		float y = 0;

		std::vector<Vertex> vertices;
		std::vector<uint> indices;

		vertices.reserve(segment);
		indices.reserve(triangleSize * 3);

		for (usize i = 0; i < segment; i++)
		{
			Vertex vertex;
			vertex.position = model * Vector2(position.x + x, position.y + y);
			// vertex.textureCoord = Vector2(x / length, y / length);
			vertex.color = color;

			float tx = -y;
			float ty = x;

			x += tx * tangetialFactor;
			y += ty * tangetialFactor;

			x *= radialFactor;
			y *= radialFactor;

			vertices.push_back(std::move(vertex));
		}

		for (usize i = 0, offset = 0; i < triangleSize; ++i)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
			offset += 3;
		}

		Draw(vertices, indices, whiteTexture);
	}

	void Renderer::DrawArc(
		const Matrix3& model, 
		float radius, 
		float beginAngle, 
		float sweepAngle, 
		int segment, 
		const Vector4& color
	)
	{
		SwitchDrawingMode(DrawingMode::Geometry);

		if (segment < 3)
			segment = 3;

		Vector2 position(0, 0);

		// // TODO: simplify this equation
		float theta = Math::twoPi / ((segment - 1) / (sweepAngle / Math::twoPi));

		float tangetialFactor = Math::Tan(theta);
		float radialFactor = Math::Cos(theta);

		Vector2 walker = Vector2::CreatePolar(radius, beginAngle);

		std::vector<Vertex> vertices;
		std::vector<uint> indices;

		vertices.reserve(segment + 1);
		indices.reserve(segment);

		Vertex vertex;
		vertex.position = model * position;
		vertex.color = color;
		vertices.push_back(vertex);

		for (usize i = 0; i < segment; i++)
		{
			Vertex vertex;
			vertex.position = model * Vector2(position.x + walker.x, position.y + walker.y);
			vertex.color = color;

			float tx = -walker.y;
			float ty = walker.x;

			walker.x += tx * tangetialFactor;
			walker.y += ty * tangetialFactor;

			walker.x *= radialFactor;
			walker.y *= radialFactor;

			vertices.push_back(std::move(vertex));
		}

		for (usize i = 0; i < segment - 1; ++i)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
		}

		indices.push_back(0);
		indices.push_back(segment);
		indices.push_back(1);

		Draw(vertices, indices, whiteTexture);
	}

	// 2D lines

	void Renderer::DrawLineTerminator(
		const Matrix3& model, 
		const Vector2& position0, 
		const Vector2& position1, 
		float thickness, 
		const Vector4& color
	)
	{
		SwitchDrawingMode(DrawingMode::Geometry);

		Vector2 offsetposition = thickness * Vector2::Normalize((position0 - position1));
		Vector2 perpenposition = thickness * Vector2::Normalize((position1 - position0).GetPerpendicular());

		DrawQuad(
			model, 
			position1 + perpenposition, 
			position0 + perpenposition + offsetposition, 
			position0 - perpenposition + offsetposition,
			position1 - perpenposition, 
			color
		);
	}

	void Renderer::DrawLineAnchor(
		const Matrix3& transform, 
		const Vector2& p0, 
		const Vector2& p1, 
		const Vector2& p2, 
		float w, 
		const Vector4& c
	)
	{
		SwitchDrawingMode(DrawingMode::Geometry);

		Vector2 i0 = transform * Vector2::zero;
		Vector2 l0 = transform * p0;
		Vector2 l1 = transform * p1;
		Vector2 l2 = transform * p2;
		Vector2 t0 = (l1 - l0).GetPerpendicular();
		Vector2 t2 = (l2 - l1).GetPerpendicular();

		if (0 < ((l1.x - l0.x) * (l2.y - l0.y) - (l2.x - l0.x) * (l1.y - l0.y)))
		{
			t0 = -t0;
			t2 = -t2;
		}

		t0.SetLength(w);
		t2.SetLength(w);

		Vector2 u0 = (l0 + t0);
		Vector2 u1 = (l2 + t2);
		Vector2 n0 = (l0 - t0);
		Vector2 n1 = (l2 - t2);
		Vector2 c0 = (l1 + t0);
		Vector2 c1 = (l1 + t2);
		Vector2 d0 = (l1 - t0);
		Vector2 d1 = (l1 - t2);
		Vector2 e0c = (l1 - l0);
		e0c.SetLength(w * 2);
		Vector2 e0 = (e0c + c0);
		Vector2 e1c = (l1 - l2);
		e1c.SetLength(w * 2);
		Vector2 e1 = (e1c + c1);

		auto areLinesIntersected = [](const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3,
									  Vector2& p4) -> bool
		{
			float denom = (p3.y - p2.y) * (p1.x - p0.x) - (p3.x - p2.x) * (p1.y - p0.y);
			float numea = (p3.x - p2.x) * (p0.y - p2.y) - (p3.y - p2.y) * (p0.x - p2.x);
			float numeb = (p1.x - p0.x) * (p0.y - p2.y) - (p1.y - p0.y) * (p0.x - p2.x);

			float denomAbs = Math::Abs(denom);
			float numeaAbs = Math::Abs(numea);
			float numebAbs = Math::Abs(numeb);

			if (numeaAbs < Math::epsilon && numebAbs < Math::epsilon && denomAbs < Math::epsilon)
			{
				p4 = Vector2::Lerp(p0, p1, 0.5);
				return true;
			}

			if (denomAbs < Math::epsilon)
				return false;

			float mua = numea / denom;
			float mub = numeb / denom;

			if (mua < 0 || mua > 1 || mub < 0 || mub > 1)
			{
				return false;
			}

			float muax = numea / denom;
			p4 = muax * (p1 - p0);
			p4 = p4 + p0;
			return true;
		};

		bool intersected = areLinesIntersected(c0, e0, c1, e1, i0);

		std::vector<Vertex> vertices = {
			Vertex{{e0.x, e0.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 0
			Vertex{{e1.x, e1.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 1
			Vertex{{u0.x, u0.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 2
			Vertex{{u1.x, u1.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 3
			Vertex{{n0.x, n0.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 4
			Vertex{{n1.x, n1.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 5
			Vertex{{c0.x, c0.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 6
			Vertex{{c1.x, c1.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 7
			Vertex{{d0.x, d0.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 8
			Vertex{{d1.x, d1.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 9
			Vertex{{i0.x, i0.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 10
			Vertex{{l1.x, l1.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 11
		};

		std::vector<uint> indices = {
			2, 6, 8, 8, 4, 2, 7, 3, 5, 5, 9, 7, 6, 7, 11, // mid
			6, 0, 1, 1, 7, 6,
		};

		if (intersected)
		{
			indices.resize(18);
			indices[15] = 6;
			indices[16] = 10;
			indices[17] = 7;
		}

		Draw(vertices, indices, whiteTexture);
	}

	void Renderer::DrawLine(
		const Matrix3& model, 
		const Vector2& position0, 
		const Vector2& position1, 
		float thickness, 
		const Vector4& color
	)
	{
		SwitchDrawingMode(DrawingMode::Geometry);

		Vector2 midposition = Vector2::Lerp(position0, position1, 0.5);

		DrawLineTerminator(model, position0, midposition, thickness, color);
		DrawLineTerminator(model, position1, midposition, thickness, color);
	}

	void Renderer::DrawPolyLine(
		const Matrix3& 
		transform, 
		const std::vector<Vector2>& ps, 
		float w, 
		const Vector4& c
	)
	{
		SwitchDrawingMode(DrawingMode::Geometry);

		if (ps.size() > 1)
		{
			std::vector<Vector2> ms;

			for (usize i = 0; i < ps.size() - 1; ++i)
				ms.push_back(Vector2::Lerp(ps[i], ps[i + 1], 0.5));

			for (usize i = 1; i < ms.size(); ++i)
				DrawLineAnchor(transform, ms[i - 1], ps[i], ms[i], w, c);

			DrawLineTerminator(transform, ps.front(), ms.front(), w, c);
			DrawLineTerminator(transform, ps.back(), ms.back(), w, c);
		}
	}

	Vector2 Renderer::MeasureText(
		const std::string& text, 
		Font* font, 
		float fontSize,
		float letterSpacing,
		float wordSpacing,
		float lineSpacing
	)
	{
		Vector2 positioner;
		float maxWidth = 0;
		usize column = 0;

		for (usize i = 0; i < text.size(); ++i)
		{
			Glyph glyph = font->glyphMap[text[i]];

			if (text[i] == ' ')
			{
				positioner.x += wordSpacing;
				continue;
			}
			else if (text[i] == '\n')
			{
				if (column > 0)
					positioner.x -= letterSpacing;

				maxWidth = Math::Max(maxWidth, positioner.x);
				positioner.y -= 1 + lineSpacing;
				positioner.x = 0;
				column = 0;
				continue;
			}

			positioner.x += glyph.size.x + font->GetKerning(text[i], text[i + 1]) - (font->glyphPadding * 2) + letterSpacing;
			++column;
		}
		
		positioner.y -= 1;

		return Vector2(Math::Max(positioner.x + (column > 0 ? -letterSpacing : 0), maxWidth), positioner.y) * fontSize;
	}

	void Renderer::DrawText(
		const Matrix3& model, 
		const Vector2& position, 
		const std::string& text, 
		Font* font, 
		float fontSize,
		float letterSpacing,
		float wordSpacing,
		float lineSpacing,
		const Vector4& color
	)
	{
		DrawRectangle(model, position, MeasureText(text, font, fontSize), Vector4(1, 1, 0, 0.5));
		
		SwitchDrawingMode(DrawingMode::Text);

		Vector2 positioner;
		positioner.x = -font->glyphPadding;
		usize column = 0;

		for (usize i = 0; i < text.size(); ++i)
		{
			Glyph glyph = font->glyphMap[text[i]];

			if (text[i] == ' ')
			{
				positioner.x += wordSpacing;
				continue;
			}
			else if (text[i] == '\n')
			{
				positioner.y -= 1 + lineSpacing;
				positioner.x = -font->glyphPadding;
				column = 0;
				continue;
			}
			
			DrawTextGlyph(
				model, 
				position + (positioner - Vector2(font->glyphPadding * 2 * column, glyph.ascent)) * fontSize, 
				glyph.size * fontSize,
				font->glyphBuffers[glyph.bufferIndex].texture, 
				glyph.texturePosition, 
				glyph.textureSize,
				color
			);

			positioner.x += glyph.size.x + font->GetKerning(text[i], text[i + 1]) + letterSpacing;
			++column;
		}
	}

	void Renderer::DrawTextGlyph(
		const Matrix3& model, 
		const Vector2& position, 
		const Vector2& size, 
		Texture2D* texture,
		const Vector2& texturePosition, 
		const Vector2& textureSize, 
		const Vector4& color
	)
	{
		SwitchDrawingMode(DrawingMode::Text);

		Draw(
			{
				Vertex(model * position, Vector4(color.x, color.y, color.z, color.w), texturePosition),
				Vertex(model * Vector2(position.x + size.x, position.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x + textureSize.x, texturePosition.y)),
				Vertex(model * Vector2(position.x + size.x, position.y - size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x + textureSize.x, texturePosition.y + textureSize.y)),
				Vertex(model * Vector2(position.x, position.y - size.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePosition.x, texturePosition.y + textureSize.y)),
			},
			{
				0, 1, 2, 2, 3, 0,
			},
			texture
		);
	}
}
