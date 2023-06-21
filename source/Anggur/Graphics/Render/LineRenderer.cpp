#include "Anggur/Graphics/Render/LineRenderer.h"

namespace Anggur {
	LineRenderer::LineRenderer() {
		vertices.assign(batchVertex, LineVertex{});

		indices.assign(batchVertex * batchIndexMultiplier, 0);

		vertexArray.bind();
		vertexArray.setLayout({
			{VertexDataType::float_, 2, "aPosition"},
			{VertexDataType::float_, 2, "aPositionA"},
			{VertexDataType::float_, 2, "aPositionB"},
			{VertexDataType::float_, 2, "aQuadrant"},
			{VertexDataType::float_, 1, "aThickness"},
			{VertexDataType::float_, 1, "aSharpness"},
			{VertexDataType::float_, 4, "aColor"},
		});

		vertexBuffer.bind();
		vertexBuffer.setCapacity(vertexArray.getStride() * vertices.size());

		indexBuffer.bind();
		indexBuffer.setCapacity(sizeof(unsigned int) * indices.size());

		shader.bind();
		shader.setVertexSource(R"(
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

			void main() 
			{
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
		shader.setFragmentSource(R"(
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
				float sizeX = abs(vPositionA.x - vPositionB.x);
				float sizeY = abs(vPositionA.y - vPositionB.y);
				float scale = 1.0f / (max(sizeX, sizeY) + 2.0f * vSharpness + vThickness);
				float sharpness = 2.0f * scale * vSharpness;
				float thickness = scale * vThickness;

				vec2 translation = 0.5f * vec2(sizeX, sizeY);

				vec2 a = 2.0f * scale * (vPositionA - translation);
				vec2 b = 2.0f * scale * (vPositionB - translation);

				vec2 ba = b - a;
				vec2 pa = vQuadrant - a;
				float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
				float distance = length(pa - h * ba);
				float mask = 1.0f - smoothstep(thickness, thickness + sharpness, abs(distance));

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

	void LineRenderer::setView(const Matrix3& newView) {
		view = newView;
	}

	void LineRenderer::add(
		const Vector2& positionA, const Vector2& positionB, float thickness, float sharpness, const Vector4& color
	) {
		if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > indices.size()) {
			flush();
		}

		Vector2 position(Math::min(positionA.x, positionB.x), Math::min(positionA.y, positionB.y));
		Vector2 size = Vector2(Math::max(positionA.x, positionB.x), Math::max(positionA.y, positionB.y)) - position;

		float edge = 0.5f * thickness + sharpness;
		float doubleSharpness = 2.0f * sharpness + thickness;
		float inverseMax = 1.0f / (Math::max(size.x, size.y) + doubleSharpness);
		float xAxis = inverseMax * (size.x + doubleSharpness);
		float yAxis = inverseMax * (size.y + doubleSharpness);

		vertices[vertexOffset + 0].position = Vector2(position.x - edge, position.y - edge);
		vertices[vertexOffset + 1].position = Vector2(position.x + size.x + edge, position.y - edge);
		vertices[vertexOffset + 2].position = Vector2(position.x + size.x + edge, position.y + size.y + edge);
		vertices[vertexOffset + 3].position = Vector2(position.x - edge, position.y + size.y + edge);

		vertices[vertexOffset + 0].positionA = positionA - position;
		vertices[vertexOffset + 1].positionA = positionA - position;
		vertices[vertexOffset + 2].positionA = positionA - position;
		vertices[vertexOffset + 3].positionA = positionA - position;

		vertices[vertexOffset + 0].positionB = positionB - position;
		vertices[vertexOffset + 1].positionB = positionB - position;
		vertices[vertexOffset + 2].positionB = positionB - position;
		vertices[vertexOffset + 3].positionB = positionB - position;

		vertices[vertexOffset + 0].quadrant.set(-xAxis, -yAxis);
		vertices[vertexOffset + 1].quadrant.set(+xAxis, -yAxis);
		vertices[vertexOffset + 2].quadrant.set(+xAxis, +yAxis);
		vertices[vertexOffset + 3].quadrant.set(-xAxis, +yAxis);

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

	void LineRenderer::flush() {
		// Early exit if no vertices to draw
		if (vertexOffset == 0)
			return;

		shader.bind();
		shader.setUniformMatrix3("uView", view);

		vertexArray.bind();

		vertexBuffer.bind();
		vertexBuffer.setData(sizeof(LineVertex) * vertexOffset, vertices.data());

		indexBuffer.bind();
		indexBuffer.setData(sizeof(unsigned int) * indexOffset, indices.data());

		glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

		vertexOffset = 0;
		indexOffset = 0;
	}
}
