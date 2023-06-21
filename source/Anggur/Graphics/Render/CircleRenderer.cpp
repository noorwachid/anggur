#include "Anggur/Graphics/Render/CircleRenderer.h"

namespace Anggur {
	CircleRenderer::CircleRenderer() {
		vertices.assign(batchVertex, CircleVertex{});

		indices.assign(batchVertex * batchIndexMultiplier, 0);

		vertexArray.bind();
		vertexArray.setLayout({
			{VertexDataType::float_, 2, "aPosition"},
			{VertexDataType::float_, 2, "aQuadrant"},
			{VertexDataType::float_, 1, "aRadius"},
			{VertexDataType::float_, 1, "aThickness"},
			{VertexDataType::float_, 1, "aSharpness"},
			{VertexDataType::float_, 4, "aColor"},
		});

		vertexBuffer.bind();
		vertexBuffer.setCapacity(sizeof(CircleVertex) * vertices.size());

		indexBuffer.bind();
		indexBuffer.setCapacity(sizeof(unsigned int) * indices.size());

		shader.bind();
		shader.setVertexSource(R"(
			#version 330 core

			layout (location = 0) in vec2 aPosition;
			layout (location = 1) in vec2 aQuadrant;
			layout (location = 2) in float aRadius;
			layout (location = 3) in float aThickness;
			layout (location = 4) in float aSharpness;
			layout (location = 5) in vec4 aColor;

			out vec2 vQuadrant;
			out float vRadius;
			out float vThickness;
			out float vSharpness;
			out vec4 vColor;

			uniform mat3 uView;

			void main() 
			{
				gl_Position.xyz = uView * vec3(aPosition, 1.0f);
				gl_Position.w = 1.0f;

				vQuadrant = aQuadrant;
				vRadius = aRadius;
				vThickness = aThickness;
				vSharpness = aSharpness;
				vColor = aColor;
			}
		)");
		shader.setFragmentSource(R"(
			#version 330 core
			
			in vec2 vQuadrant;
			in float vRadius;
			in float vThickness;
			in float vSharpness;
			in vec4 vColor;

			out vec4 fColor;
			
			void main() 
			{
				float scale = 1.0f / (vRadius + vSharpness);
				float radius = scale * vRadius;
				float sharpness = scale * vSharpness;
				float thickness = scale * vThickness;
				float distance = length(vQuadrant) - radius;

				float outerCircumference = 1.0f - smoothstep(0.0f, sharpness, distance);
				float innerCircumference = 1.0f - smoothstep(thickness, thickness + sharpness, abs(distance));
				float mask = min(outerCircumference, innerCircumference);

				fColor = vColor;
				fColor.w *= mask;

				if (fColor.w == 0.0f) 
				{
					discard;
				}
			}
		)");
		shader.compile();
	}

	void CircleRenderer::setView(const Matrix3& newView) {
		view = newView;
	}

	void CircleRenderer::add(
		const Vector2& position, float radius, float thickness, float sharpness, const Vector4& color
	) {
		if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > vertices.size()) {
			flush();
		}

		float spread = radius + sharpness;

		vertices[vertexOffset + 0].position = Vector2(position.x - spread, position.y - spread);
		vertices[vertexOffset + 1].position = Vector2(position.x + spread, position.y - spread);
		vertices[vertexOffset + 2].position = Vector2(position.x + spread, position.y + spread);
		vertices[vertexOffset + 3].position = Vector2(position.x - spread, position.y + spread);

		vertices[vertexOffset + 0].quadrant.set(-1, -1);
		vertices[vertexOffset + 1].quadrant.set(+1, -1);
		vertices[vertexOffset + 2].quadrant.set(+1, +1);
		vertices[vertexOffset + 3].quadrant.set(-1, +1);

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

	void CircleRenderer::flush() {
		shader.bind();
		shader.setUniformMatrix3("uView", view);

		vertexArray.bind();

		vertexBuffer.bind();
		vertexBuffer.setData(sizeof(CircleVertex) * vertexOffset, vertices.data());

		indexBuffer.bind();
		indexBuffer.setData(sizeof(unsigned int) * indexOffset, indices.data());

		glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

		vertexOffset = 0;
		indexOffset = 0;
	}
}
