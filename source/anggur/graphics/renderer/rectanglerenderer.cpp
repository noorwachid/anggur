#include "anggur/graphics/renderer/rectanglerenderer.h"
#include "anggur/graphics/api.h"

namespace Anggur {
	RoundedRectangleRenderer::RoundedRectangleRenderer() {
		vertices.assign(batchVertex, RoundedRectangleVertex{});

		indices.assign(batchVertex * batchIndexMultiplier, 0);

		vertexArray.bind();
		vertexArray.setLayout({
			{VertexDataType::Float, 2, "aPosition"},
			{VertexDataType::Float, 2, "aSize"},
			{VertexDataType::Float, 2, "aQuadrant"},
			{VertexDataType::Float, 1, "aRadius"},
			{VertexDataType::Float, 1, "aThickness"},
			{VertexDataType::Float, 1, "aSharpness"},
			{VertexDataType::Float, 4, "aColor"},
		});

		vertexBuffer.bind();
		vertexBuffer.setCapacity(vertexArray.getStride() * vertices.size());

		indexBuffer.bind();
		indexBuffer.setCapacity(sizeof(unsigned int) * indices.size());

		shader.bind();
		shader.setVertexSource(R"(
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

			uniform mat3 uView;

			void main() 
			{
				gl_Position.xyz = uView * vec3(aPosition, 1.0f);
				gl_Position.w = 1.0f;

				vSize = aSize;
				vQuadrant = aQuadrant;
				vRadius = aRadius;
				vThickness = aThickness;
				vSharpness = aSharpness;
				vColor = aColor;
			}
		)");
		shader.setFragmentSource(R"(
			in vec2 vSize;
			in vec2 vQuadrant;
			in float vRadius;
			in float vThickness;
			in float vSharpness;
			in vec4 vColor;

			out vec4 fColor;

			float RoundRectangle(vec2 position, vec2 size, float radius)
			{
				vec2 edge = abs(position) - size + radius;
				float outside = length(max(edge, 0.0f));
				float inside = min(max(edge.x, edge.y), 0.0f);

				return inside + outside - radius;
			}
			
			void main() 
			{
				float scale = 1.0f / (max(vSize.x, vSize.y) + (vSharpness * 2.0f));
				float doubleScale = 2.0f * scale;

				float sharpness = doubleScale * vSharpness;
				float thickness = doubleScale * vThickness;
				float radius = doubleScale * vRadius;

				vec2 size = scale * vSize;

				float distance = RoundRectangle(vQuadrant, size, radius);

				float outerCircumference = 1.0f - smoothstep(0.0f, sharpness, distance);
				float innerCircumference = 1.0f - smoothstep(thickness, thickness + sharpness, abs(distance));
				float mask = min(innerCircumference, outerCircumference);

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

	void RoundedRectangleRenderer::setView(const Matrix3& view) {
		this->view = view;
	}

	void RoundedRectangleRenderer::add(
		const Vector2& position, const Vector2& size, float radius, float thickness, float sharpness,
		const Vector4& color
	) {
		if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > vertices.size()) {
			flush();
		}

		float doubleSharpness = 2.0f * sharpness;
		float inverseMax = 1.0f / (Math::max(size.x, size.y) + doubleSharpness);
		float xAxis = inverseMax * (size.x + doubleSharpness);
		float yAxis = inverseMax * (size.y + doubleSharpness);

		vertices[vertexOffset + 0].position = Vector2(position.x - sharpness, position.y - sharpness);
		vertices[vertexOffset + 1].position = Vector2(position.x + size.x + sharpness, position.y - sharpness);
		vertices[vertexOffset + 2].position =
			Vector2(position.x + size.x + sharpness, position.y + size.y + sharpness);
		vertices[vertexOffset + 3].position = Vector2(position.x - sharpness, position.y + size.y + sharpness);

		vertices[vertexOffset + 0].size = size;
		vertices[vertexOffset + 1].size = size;
		vertices[vertexOffset + 2].size = size;
		vertices[vertexOffset + 3].size = size;

		vertices[vertexOffset + 0].quadrant.set(-xAxis, -yAxis);
		vertices[vertexOffset + 1].quadrant.set(+xAxis, -yAxis);
		vertices[vertexOffset + 2].quadrant.set(+xAxis, +yAxis);
		vertices[vertexOffset + 3].quadrant.set(-xAxis, +yAxis);

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

	void RoundedRectangleRenderer::flush() {
		// Early exit if no vertices to draw
		if (vertexOffset == 0)
			return;

		shader.bind();
		shader.setUniformMatrix3("uView", view);

		vertexArray.bind();

		vertexBuffer.bind();
		vertexBuffer.setData(sizeof(RoundedRectangleRenderer) * vertexOffset, vertices.data());

		indexBuffer.bind();
		indexBuffer.setData(sizeof(unsigned int) * indexOffset, indices.data());

		glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

		vertexOffset = 0;
		indexOffset = 0;
	}
}
