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
	struct LineVertex 
	{
		Vector2 position;
		Vector2 positionA;
		Vector2 positionB;
		Vector2 quadrant;
		float thickness = 0.5f;
		float sharpness = 0.01f;
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class LineRenderer
	{
	public:
		LineRenderer() 
		{
			vertices.assign(batchVertex, LineVertex{});

			indices.assign(batchVertex * batchIndexMultiplier, 0);

			vertexArray.Bind();
			vertexArray.SetLayout({ 
				{ VertexDataType::Float, 2, "aPosition" },
				{ VertexDataType::Float, 2, "aPositionA" },
				{ VertexDataType::Float, 2, "aPositionB" },
				{ VertexDataType::Float, 2, "aQuadrant" },
				{ VertexDataType::Float, 1, "aThickness" },
				{ VertexDataType::Float, 1, "aSharpness" },
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
				layout (location = 1) in vec2 aPositionA;
				layout (location = 2) in vec2 aPositionB;
				layout (location = 3) in vec2 aQuadrant;
				layout (location = 4) in float aThickness;
				layout (location = 5) in float aSharpness;
				layout (location = 6) in vec4 aColor;

				out vec2 vPositionA;
				out vec2 vPositionB;
				out vec2 vQuadrant;
				out float vThickness;
				out float vSharpness;
				out vec4 vColor;

				uniform mat3 uView;

				void main() {
					gl_Position.xyz = uView * vec3(aPosition, 1.0f);
					gl_Position.w = 1.0f;

					vPositionA = aPositionA;
					vPositionB = aPositionB;
					vQuadrant = aQuadrant;
					vThickness = aThickness;
					vSharpness = aSharpness;
					vColor = aColor;
				}
			)");
			shader.SetFragmentSource(R"(
				#version 330 core
				
				in vec2 vPositionA;
				in vec2 vPositionB;
				in vec2 vQuadrant;
				in float vThickness;
				in float vSharpness;
				in vec4 vColor;

				out vec4 fColor;
				
				void main() 
				{
					vec2 a = vPositionA;
					vec2 b = vPositionB;
					vec2 c = vQuadrant;

					vec2 ab = b - a;
					vec2 ac = c - a;
					vec2 ab_hat = normalize(ab);
					vec2 d = dot(ac,ab_hat) * ab_hat + a;
					d = max(d, min(a, b));
					d = min(d, max(a, b));
					float dist = distance(c, d);
					float mask = smoothstep(0.005, .009, dist);

					fColor = vColor;
					fColor.w *= mask;

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

		void Add(const Vector2& positionA, const Vector2& positionB, float thickness, float sharpness, const Vector4& color)
		{
			if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > vertices.size())
			{
				Draw();
			}

			Vector2 position(Math::Min(positionA.x, positionB.x), Math::Min(positionA.y, positionB.y));
			Vector2 size(Math::Max(positionA.x, positionB.x), Math::Max(positionA.y, positionB.y));

			std::cout << "POS: " << position.ToString() << "\n";
			std::cout << "SIZ: " << size.ToString() << "\n";

			float maxSize = Math::Max(size.x, size.y);
			float halfMaxSize = maxSize * 0.5;

			vertices[vertexOffset + 0].position = Vector2(position.x - halfMaxSize,           position.y - halfMaxSize);
			vertices[vertexOffset + 1].position = Vector2(position.x + maxSize + halfMaxSize, position.y - halfMaxSize);
			vertices[vertexOffset + 2].position = Vector2(position.x + maxSize + halfMaxSize, position.y + maxSize + halfMaxSize);
			vertices[vertexOffset + 3].position = Vector2(position.x - halfMaxSize,           position.y + maxSize + halfMaxSize);

			vertices[vertexOffset + 0].positionA = positionA;
			vertices[vertexOffset + 1].positionA = positionA;
			vertices[vertexOffset + 2].positionA = positionA;
			vertices[vertexOffset + 3].positionA = positionA;

			vertices[vertexOffset + 0].positionB = positionB;
			vertices[vertexOffset + 1].positionB = positionB;
			vertices[vertexOffset + 2].positionB = positionB;
			vertices[vertexOffset + 3].positionB = positionB;

			vertices[vertexOffset + 0].quadrant.Set(-1, -1);
			vertices[vertexOffset + 1].quadrant.Set(+1, -1);
			vertices[vertexOffset + 2].quadrant.Set(+1, +1);
			vertices[vertexOffset + 3].quadrant.Set(-1, +1);

			vertices[vertexOffset + 0].thickness = thickness;
			vertices[vertexOffset + 1].thickness = thickness;
			vertices[vertexOffset + 2].thickness = thickness;
			vertices[vertexOffset + 3].thickness = thickness;

			vertices[vertexOffset + 0].sharpness = sharpness;
			vertices[vertexOffset + 1].sharpness = sharpness;
			vertices[vertexOffset + 2].sharpness = sharpness;
			vertices[vertexOffset + 3].sharpness = sharpness;

			vertices[vertexOffset + 0].color = color;
			vertices[vertexOffset + 1].color = color;
			vertices[vertexOffset + 2].color = color;
			vertices[vertexOffset + 3].color = color;

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

			shader.Bind();
			shader.SetUniformMatrix3("uView", view);

			vertexArray.Bind();

			vertexBuffer.Bind();
			vertexBuffer.SetData(sizeof(LineRenderer) * vertexOffset, vertices.data());

			indexBuffer.Bind();
			indexBuffer.SetData(sizeof(uint) * indexOffset, indices.data());

			glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

			vertexOffset = 0;
			indexOffset = 0;
		}


	private:
		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		Shader shader;

		Matrix3 view;

		std::vector<LineVertex> vertices;
		std::vector<uint> indices;

		usize vertexOffset = 0;
		usize indexOffset = 0;

		usize drawCount = 0;

		usize batchVertex = 512;
		usize batchIndexMultiplier = 2;
	};
}


