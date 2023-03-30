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
	struct RCRVertex 
	{
		Vector2 position;
		Vector2 size;
		Vector2 quadrant;
		float sharpness = 0.01f;
		float thickness = 0.5f;
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class RCRPipeline
	{
	public:
		RCRPipeline() 
		{
			vertices.assign(
				batchVertex, RCRVertex{}
			);

			indices.assign(batchVertex * batchIndexMultiplier, 0);

			vertexBuffer.Bind();
			vertexBuffer.SetCapacity(sizeof(RCRVertex) * vertices.size());

			vertexArray.Bind();
			vertexArray.SetAttribute(0, 2, GL_FLOAT, sizeof(RCRVertex), offsetof(RCRVertex, position));
			vertexArray.SetAttribute(1, 2, GL_FLOAT, sizeof(RCRVertex), offsetof(RCRVertex, size));
			vertexArray.SetAttribute(2, 2, GL_FLOAT, sizeof(RCRVertex), offsetof(RCRVertex, quadrant));
			vertexArray.SetAttribute(3, 1, GL_FLOAT, sizeof(RCRVertex), offsetof(RCRVertex, sharpness));
			vertexArray.SetAttribute(4, 1, GL_FLOAT, sizeof(RCRVertex), offsetof(RCRVertex, thickness));
			vertexArray.SetAttribute(5, 4, GL_FLOAT, sizeof(RCRVertex), offsetof(RCRVertex, color));

			indexBuffer.Bind();
			indexBuffer.SetCapacity(sizeof(uint) * indices.size());

			shader.Bind();
			shader.SetVertexSource(R"(
				#version 330 core

				layout (location = 0) in vec2 aPosition;
				layout (location = 1) in vec2 aSize;
				layout (location = 2) in vec2 aQuadrant;
				layout (location = 3) in float aSharpness;
				layout (location = 4) in float aThickness;
				layout (location = 5) in vec4 aColor;

				out vec2 vSize;
				out vec2 vQuadrant;
				out float vSharpness;
				out float vThickness;
				out vec4 vColor;

				void main() {
					gl_Position.xy = aPosition;
					gl_Position.z = 0.0f;
					gl_Position.w = 1.0f;

					vSize = aSize;
					vQuadrant = aQuadrant;
					vSharpness = aSharpness;
					vThickness = aThickness;
					vColor = aColor;
				}
			)");
			shader.SetFragmentSource(
				R"(
				#version 330 core
				
				in vec2 vSize;
				in vec2 vQuadrant;
				in float vSharpness;
				in float vThickness;
				in vec4 vColor;

				out vec4 fColor;

				float CalculateRC(vec2 center, vec2 size, float radius)
				{
					return length(max(abs(center) - size + radius, 0.0)) - radius;
				}

				
				void main() 
				{
					vec2 halfSize = vSize / 2.0f;
					float distance = CalculateRC(vQuadrant - vec2(halfSize.x, halfSize.y), halfSize, 0.2f);

					fColor = vec4(step(distance, 0.0));
				}
			)"
			);
			shader.Compile();
		}

		void Add(const Vector2& position, const Vector2& size, float sharpness = 0.01, float thickness = 1.0)
		{
			if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > vertices.size())
			{
				Draw();
			}

			Matrix3 r = Matrix3::CreateRotation(Math::pi / 8);
			// Matrix3 r;

			vertices[vertexOffset + 0].position = r * Vector2(position.x - 1.0, position.y - 1.0);
			vertices[vertexOffset + 1].position = r * Vector2(position.x + 1.0, position.y - 1.0);
			vertices[vertexOffset + 2].position = r * Vector2(position.x + 1.0, position.y + 1.0);
			vertices[vertexOffset + 3].position = r * Vector2(position.x - 1.0, position.y + 1.0);

			vertices[vertexOffset + 0].quadrant.Set(-1, -1);
			vertices[vertexOffset + 1].quadrant.Set(+1, -1);
			vertices[vertexOffset + 2].quadrant.Set(+1, +1);
			vertices[vertexOffset + 3].quadrant.Set(-1, +1);

			vertices[vertexOffset + 0].sharpness = sharpness;
			vertices[vertexOffset + 1].sharpness = sharpness;
			vertices[vertexOffset + 2].sharpness = sharpness;
			vertices[vertexOffset + 3].sharpness = sharpness;

			vertices[vertexOffset + 0].thickness = thickness;
			vertices[vertexOffset + 1].thickness = thickness;
			vertices[vertexOffset + 2].thickness = thickness;
			vertices[vertexOffset + 3].thickness = thickness;

			vertices[vertexOffset + 0].size = size;
			vertices[vertexOffset + 1].size = size;
			vertices[vertexOffset + 2].size = size;
			vertices[vertexOffset + 3].size = size;

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
			shader.Bind();

			vertexArray.Bind();

			vertexBuffer.Bind();
			vertexBuffer.setData(sizeof(RCRVertex) * vertexOffset, vertices.data());

			indexBuffer.Bind();
			indexBuffer.setData(sizeof(uint) * indexOffset, indices.data());

			glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

			vertexOffset = 0;
			indexOffset = 0;
		}


	private:
		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		Shader shader;

		std::vector<RCRVertex> vertices;
		std::vector<uint> indices;

		usize vertexOffset = 0;
		usize indexOffset = 0;

		usize drawCount = 0;

		usize batchVertex = 512;
		usize batchIndexMultiplier = 2;
	};
}

