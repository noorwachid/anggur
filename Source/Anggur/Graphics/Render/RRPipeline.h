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
	struct RRVertex 
	{
		Vector2 position;
		Vector2 size;
		Vector2 quadrant;
		float radius = 0.0f;
		float thickness = 0.5f;
		float sharpness = 0.01f;
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class RRPipeline
	{
	public:
		RRPipeline() 
		{
			vertices.assign(
				batchVertex, RRVertex{}
			);

			indices.assign(batchVertex * batchIndexMultiplier, 0);

			vertexBuffer.Bind();
			vertexBuffer.SetCapacity(sizeof(RRVertex) * vertices.size());

			vertexArray.Bind();
			vertexArray.SetAttribute(0, 2, GL_FLOAT, sizeof(RRVertex), offsetof(RRVertex, position));
			vertexArray.SetAttribute(1, 2, GL_FLOAT, sizeof(RRVertex), offsetof(RRVertex, size));
			vertexArray.SetAttribute(2, 2, GL_FLOAT, sizeof(RRVertex), offsetof(RRVertex, quadrant));
			vertexArray.SetAttribute(3, 1, GL_FLOAT, sizeof(RRVertex), offsetof(RRVertex, radius));
			vertexArray.SetAttribute(4, 1, GL_FLOAT, sizeof(RRVertex), offsetof(RRVertex, thickness));
			vertexArray.SetAttribute(5, 1, GL_FLOAT, sizeof(RRVertex), offsetof(RRVertex, sharpness));
			vertexArray.SetAttribute(6, 4, GL_FLOAT, sizeof(RRVertex), offsetof(RRVertex, color));

			indexBuffer.Bind();
			indexBuffer.SetCapacity(sizeof(uint) * indices.size());

			shader.Bind();
			shader.SetVertexSource(R"(
				#version 330 core

				layout (location = 0) in vec2 aPosition;
				layout (location = 1) in vec2 aSize;
				layout (location = 2) in vec2 aQuadrant;
				layout (location = 3) in float aRadius;
				layout (location = 4) in float aThickness;
				layout (location = 5) in float aSharpness;
				layout (location = 6) in vec4 aColor;

				out vec2 vSize;
				out vec2 vQuadrant;
				out float vRadius;
				out float vThickness;
				out float vSharpness;
				out vec4 vColor;

				uniform mat3 uViewProjection;

				void main() {
					gl_Position.xyz = uViewProjection * vec3(aPosition, 1.0f);
					gl_Position.w = 1.0f;

					vSize = aSize;
					vQuadrant = aQuadrant;
					vRadius = aRadius;
					vThickness = aThickness;
					vSharpness = aSharpness;
					vColor = aColor;
				}
			)");
			shader.SetFragmentSource(R"(
				#version 330 core
				
				in vec2 vSize;
				in vec2 vQuadrant;
				in float vRadius;
				in float vThickness;
				in float vSharpness;
				in vec4 vColor;

				out vec4 fColor;

				float CalculateRR(vec2 position, vec2 size, float radius)
				{
					vec2 edge = abs(position) - size + radius;
					float outside = length(max(edge, 0.0f));
					float inside = min(max(edge.x, edge.y), 0.0f);

					return inside + outside - radius;
				}
				
				void main() 
				{
					float scale = 0.5f / max(vSize.x, vSize.y);

					float sharpness = scale * vSharpness;
					float thickness = scale * 2.0f * vThickness;
					float radius = scale * vRadius;

					vec2 size = scale * vSize;

					float distance = CalculateRR(vQuadrant + vec2(0.5f) - size, size, radius);

					float outerCircumference = 1.0f - smoothstep(0.0f, sharpness, distance);
					float innerCircumference = 1.0f - smoothstep(thickness, thickness + sharpness, abs(distance));
					float mask = min(innerCircumference, outerCircumference);

					fColor = vColor;
					fColor.w *= mask;
				}
			)");
			shader.Compile();
		}

		void SetViewProjection(const Matrix3& newViewProjection)
		{
			viewProjection = newViewProjection;
			shader.SetUniformMatrix3("uViewProjection", viewProjection);
		}

		void Add(const Vector2& position, const Vector2& size, float radius = 0.0, float thickness = 1.0, float sharpness = 0.25, const Vector4& color = Vector4(1.0, 1.0, 1.0, 1.0))
		{
			if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > vertices.size())
			{
				Draw();
			}

			float maxSize = Math::Max(size.x, size.y);
			float halfMaxSize = maxSize * 0.5;

			vertices[vertexOffset + 0].position = Vector2(position.x - halfMaxSize,           position.y - halfMaxSize);
			vertices[vertexOffset + 1].position = Vector2(position.x + maxSize + halfMaxSize, position.y - halfMaxSize);
			vertices[vertexOffset + 2].position = Vector2(position.x + maxSize + halfMaxSize, position.y + maxSize + halfMaxSize);
			vertices[vertexOffset + 3].position = Vector2(position.x - halfMaxSize,           position.y + maxSize + halfMaxSize);

			vertices[vertexOffset + 0].size = size;
			vertices[vertexOffset + 1].size = size;
			vertices[vertexOffset + 2].size = size;
			vertices[vertexOffset + 3].size = size;

			vertices[vertexOffset + 0].quadrant.Set(-1, -1);
			vertices[vertexOffset + 1].quadrant.Set(+1, -1);
			vertices[vertexOffset + 2].quadrant.Set(+1, +1);
			vertices[vertexOffset + 3].quadrant.Set(-1, +1);

			vertices[vertexOffset + 0].radius = radius;
			vertices[vertexOffset + 1].radius = radius;
			vertices[vertexOffset + 2].radius = radius;
			vertices[vertexOffset + 3].radius = radius;

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
			shader.Bind();

			vertexArray.Bind();

			vertexBuffer.Bind();
			vertexBuffer.setData(sizeof(RRVertex) * vertexOffset, vertices.data());

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

		Matrix3 viewProjection;

		std::vector<RRVertex> vertices;
		std::vector<uint> indices;

		usize vertexOffset = 0;
		usize indexOffset = 0;

		usize drawCount = 0;

		usize batchVertex = 512;
		usize batchIndexMultiplier = 2;
	};
}

