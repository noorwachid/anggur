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
	struct CircleVertex 
	{
		Vector2 position;
		Vector2 direction;
		float sharpness = 0.01f;
		float thickness = 0.5f;
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class CirclePipeline
	{
	public:
		CirclePipeline() 
		{
			vertices.assign(
				batchVertex, CircleVertex{}
			);

			indices.assign(batchVertex * batchIndexMultiplier, 0);

			vertexBuffer.Bind();
			vertexBuffer.SetCapacity(sizeof(CircleVertex) * vertices.size());

			vertexArray.Bind();
			vertexArray.SetAttribute(0, 2, GL_FLOAT, sizeof(CircleVertex), offsetof(CircleVertex, position));
			vertexArray.SetAttribute(1, 2, GL_FLOAT, sizeof(CircleVertex), offsetof(CircleVertex, direction));
			vertexArray.SetAttribute(2, 1, GL_FLOAT, sizeof(CircleVertex), offsetof(CircleVertex, sharpness));
			vertexArray.SetAttribute(3, 1, GL_FLOAT, sizeof(CircleVertex), offsetof(CircleVertex, thickness));
			vertexArray.SetAttribute(4, 4, GL_FLOAT, sizeof(CircleVertex), offsetof(CircleVertex, color));

			indexBuffer.Bind();
			indexBuffer.SetCapacity(sizeof(uint) * indices.size());

			shader.Bind();
			shader.SetVertexSource(R"(
				#version 330 core

				layout (location = 0) in vec2 aPosition;
				layout (location = 1) in vec2 aDirection;
				layout (location = 2) in float aSharpness;
				layout (location = 3) in float aThickness;
				layout (location = 4) in vec4 aColor;

				out vec2 vDirection;
				out float vSharpness;
				out float vThickness;
				out vec4 vColor;

				void main() {
					gl_Position.xy = aPosition;
					gl_Position.z = 0.0f;
					gl_Position.w = 1.0f;

					vDirection = aDirection;
					vSharpness = aSharpness;
					vThickness = aThickness;
					vColor = aColor;
				}
			)");
			shader.SetFragmentSource(
				R"(
				#version 330 core
				
				in vec2 vDirection;
				in float vSharpness;
				in float vThickness;
				in vec4 vColor;

				out vec4 fColor;
				
				void main() {

					float distance = 1.0f - length(vDirection);

					vec4 mask = vec4(smoothstep(0.0f, vSharpness, distance) * smoothstep(vThickness + vSharpness, vThickness, distance));

					fColor = vColor * mask;
				}
			)"
			);
			shader.Compile();
		}

		void Add(const Vector2& position, float radius = 0.5, float sharpness = 0.01, float thickness = 1.0)
		{
			if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > vertices.size())
			{
				Draw();
			}

			vertices[vertexOffset + 0].position = Vector2(position.x - radius, position.y - radius);
			vertices[vertexOffset + 1].position = Vector2(position.x + radius, position.y - radius);
			vertices[vertexOffset + 2].position = Vector2(position.x + radius, position.y + radius);
			vertices[vertexOffset + 3].position = Vector2(position.x - radius, position.y + radius);

			vertices[vertexOffset + 0].direction.Set(-1, -1);
			vertices[vertexOffset + 1].direction.Set(+1, -1);
			vertices[vertexOffset + 2].direction.Set(+1, +1);
			vertices[vertexOffset + 3].direction.Set(-1, +1);

			vertices[vertexOffset + 0].sharpness = sharpness;
			vertices[vertexOffset + 1].sharpness = sharpness;
			vertices[vertexOffset + 2].sharpness = sharpness;
			vertices[vertexOffset + 3].sharpness = sharpness;

			vertices[vertexOffset + 0].thickness = thickness;
			vertices[vertexOffset + 1].thickness = thickness;
			vertices[vertexOffset + 2].thickness = thickness;
			vertices[vertexOffset + 3].thickness = thickness;

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
			vertexBuffer.setData(sizeof(CircleVertex) * vertexOffset, vertices.data());

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

		std::vector<CircleVertex> vertices;
		std::vector<uint> indices;

		usize vertexOffset = 0;
		usize indexOffset = 0;

		usize drawCount = 0;

		usize batchVertex = 512;
		usize batchIndexMultiplier = 2;
	};
}
