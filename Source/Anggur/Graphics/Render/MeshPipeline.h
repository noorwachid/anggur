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
	struct MeshVertex 
	{
		Vector2 position;
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class MeshPipeline
	{
	public:
		MeshPipeline() 
		{
			vertices.assign(
				batchVertex, MeshVertex{}
			);

			indices.assign(batchVertex * batchIndexMultiplier, 0);

			vertexBuffer.Bind();
			vertexBuffer.SetCapacity(sizeof(MeshVertex) * vertices.size());

			vertexArray.Bind();
			vertexArray.SetAttribute(0, 2, GL_FLOAT, sizeof(MeshVertex), offsetof(MeshVertex, position));
			vertexArray.SetAttribute(1, 4, GL_FLOAT, sizeof(MeshVertex), offsetof(MeshVertex, color));

			indexBuffer.Bind();
			indexBuffer.SetCapacity(sizeof(uint) * indices.size());

			shader.Bind();
			shader.SetVertexSource(R"(
				#version 330 core

				layout (location = 0) in vec2 aPosition;
				layout (location = 1) in vec4 aColor;

				out vec4 vColor;

				uniform mat3 uView;

				void main() {
					gl_Position.xyz = uView * vec3(aPosition, 1.0f);
					gl_Position.w = 1.0f;

					vColor = aColor;
				}
			)");
			shader.SetFragmentSource(R"(
				#version 330 core
				
				in vec4 vColor;

				out vec4 fColor;

				void main() 
				{
					fColor = vColor;

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

		void AddRectangle(const Vector2& position, const Vector2& size, const Vector4& color)
		{
			if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > vertices.size())
			{
				Draw();
			}

			vertices[vertexOffset + 0].position = Vector2(position.x,          position.y);
			vertices[vertexOffset + 1].position = Vector2(position.x + size.x, position.y);
			vertices[vertexOffset + 2].position = Vector2(position.x + size.x, position.y + size.y);
			vertices[vertexOffset + 3].position = Vector2(position.x,          position.y + size.y);

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
			vertexBuffer.SetData(sizeof(MeshVertex) * vertexOffset, vertices.data());

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

		std::vector<MeshVertex> vertices;
		std::vector<uint> indices;

		usize vertexOffset = 0;
		usize indexOffset = 0;

		usize drawCount = 0;

		usize batchVertex = 512;
		usize batchIndexMultiplier = 2;
	};
}


