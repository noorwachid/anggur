#include "anggur/graphics/renderer/circlerenderer.h"
#include "anggur/graphics/api.h"

namespace Anggur {
	CircleRenderer::CircleRenderer() {
		_vertices.assign(_batchVertex, CircleVertex{});

		_indices.assign(_batchVertex * _batchIndexMultiplier, 0);

		_vertexArray.bind();
		_vertexArray.setLayout({
			{VertexDataType::Float, 2, "aPosition"},
			{VertexDataType::Float, 2, "aQuadrant"},
			{VertexDataType::Float, 1, "aRadius"},
			{VertexDataType::Float, 1, "aThickness"},
			{VertexDataType::Float, 1, "aSharpness"},
			{VertexDataType::Float, 4, "aColor"},
		});

		_vertexBuffer.bind();
		_vertexBuffer.setCapacity(sizeof(CircleVertex) * _vertices.size());

		_indexBuffer.bind();
		_indexBuffer.setCapacity(sizeof(unsigned int) * _indices.size());

		_shader.bind();
		_shader.setVertexSource(R"(
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
		_shader.setFragmentSource(R"(
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
		_shader.compile();
	}

	void CircleRenderer::setView(const Matrix3& newView) {
		_view = newView;
	}

	void CircleRenderer::add(
		const Vector2& position, float radius, float thickness, float sharpness, const Vector4& color
	) {
		if (_vertexOffset + 4 > _vertices.size() || _indexOffset + 6 > _vertices.size()) {
			flush();
		}

		float spread = radius + sharpness;

		_vertices[_vertexOffset + 0].position = Vector2(position.x - spread, position.y - spread);
		_vertices[_vertexOffset + 1].position = Vector2(position.x + spread, position.y - spread);
		_vertices[_vertexOffset + 2].position = Vector2(position.x + spread, position.y + spread);
		_vertices[_vertexOffset + 3].position = Vector2(position.x - spread, position.y + spread);

		_vertices[_vertexOffset + 0].quadrant.set(-1, -1);
		_vertices[_vertexOffset + 1].quadrant.set(+1, -1);
		_vertices[_vertexOffset + 2].quadrant.set(+1, +1);
		_vertices[_vertexOffset + 3].quadrant.set(-1, +1);

		_vertices[_vertexOffset + 0].radius = radius;
		_vertices[_vertexOffset + 1].radius = radius;
		_vertices[_vertexOffset + 2].radius = radius;
		_vertices[_vertexOffset + 3].radius = radius;

		_vertices[_vertexOffset + 0].thickness = thickness;
		_vertices[_vertexOffset + 1].thickness = thickness;
		_vertices[_vertexOffset + 2].thickness = thickness;
		_vertices[_vertexOffset + 3].thickness = thickness;

		_vertices[_vertexOffset + 0].sharpness = sharpness;
		_vertices[_vertexOffset + 1].sharpness = sharpness;
		_vertices[_vertexOffset + 2].sharpness = sharpness;
		_vertices[_vertexOffset + 3].sharpness = sharpness;

		_vertices[_vertexOffset + 0].color = color;
		_vertices[_vertexOffset + 1].color = color;
		_vertices[_vertexOffset + 2].color = color;
		_vertices[_vertexOffset + 3].color = color;

		_indices[_indexOffset + 0] = _vertexOffset + 0;
		_indices[_indexOffset + 1] = _vertexOffset + 1;
		_indices[_indexOffset + 2] = _vertexOffset + 2;
		_indices[_indexOffset + 3] = _vertexOffset + 2;
		_indices[_indexOffset + 4] = _vertexOffset + 3;
		_indices[_indexOffset + 5] = _vertexOffset + 0;

		_vertexOffset += 4;

		_indexOffset += 6;
	}

	void CircleRenderer::flush() {
		_shader.bind();
		_shader.setUniformMatrix3("uView", _view);

		_vertexArray.bind();

		_vertexBuffer.bind();
		_vertexBuffer.setData(sizeof(CircleVertex) * _vertexOffset, _vertices.data());

		_indexBuffer.bind();
		_indexBuffer.setData(sizeof(unsigned int) * _indexOffset, _indices.data());

		glDrawElements(GL_TRIANGLES, _indexOffset, GL_UNSIGNED_INT, nullptr);

		_vertexOffset = 0;
		_indexOffset = 0;
	}
}
