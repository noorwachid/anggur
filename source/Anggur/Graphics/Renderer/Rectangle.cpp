#include "Anggur/Graphics/Renderer/Rectangle.h"
#include "Anggur/Graphics/API.h"

namespace Anggur
{
	RectangleRenderer::RectangleRenderer()
	{
		_vertices.assign(_batchVertex, RectangleVertex{});

		_indices.assign(_batchVertex * _batchIndexMultiplier, 0);

		_vertexArray.Bind();
		_vertexArray.SetLayout({
			{VertexDataType::Float, 2, "aPosition"},
			{VertexDataType::Float, 2, "aSize"},
			{VertexDataType::Float, 2, "aQuadrant"},
			{VertexDataType::Float, 1, "aRadius"},
			{VertexDataType::Float, 1, "aThickness"},
			{VertexDataType::Float, 1, "aSharpness"},
			{VertexDataType::Float, 4, "aColor"},
		});

		_vertexBuffer.Bind();
		_vertexBuffer.SetCapacity(_vertexArray.GetStride() * _vertices.size());

		_indexBuffer.Bind();
		_indexBuffer.SetCapacity(sizeof(unsigned int) * _indices.size());

		_shader.Bind();
		_shader.SetVertexSource(R"(
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
		_shader.SetFragmentSource(R"(
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
		_shader.Compile();
	}

	void RectangleRenderer::SetView(const Matrix3& newView)
	{
		_view = newView;
	}

	void RectangleRenderer::Add(
		const Vector2& position, const Vector2& size, float radius, float thickness, float sharpness,
		const Vector4& color
	)
	{
		if (_vertexOffset + 4 > _vertices.size() || _indexOffset + 6 > _vertices.size())
		{
			Flush();
		}

		float doubleSharpness = 2.0f * sharpness;
		float inverseMax = 1.0f / (Math::Max(size.x, size.y) + doubleSharpness);
		float xAxis = inverseMax * (size.x + doubleSharpness);
		float yAxis = inverseMax * (size.y + doubleSharpness);

		_vertices[_vertexOffset + 0].position = Vector2(position.x - sharpness, position.y - sharpness);
		_vertices[_vertexOffset + 1].position = Vector2(position.x + size.x + sharpness, position.y - sharpness);
		_vertices[_vertexOffset + 2].position =
			Vector2(position.x + size.x + sharpness, position.y + size.y + sharpness);
		_vertices[_vertexOffset + 3].position = Vector2(position.x - sharpness, position.y + size.y + sharpness);

		_vertices[_vertexOffset + 0].size = size;
		_vertices[_vertexOffset + 1].size = size;
		_vertices[_vertexOffset + 2].size = size;
		_vertices[_vertexOffset + 3].size = size;

		_vertices[_vertexOffset + 0].quadrant.Set(-xAxis, -yAxis);
		_vertices[_vertexOffset + 1].quadrant.Set(+xAxis, -yAxis);
		_vertices[_vertexOffset + 2].quadrant.Set(+xAxis, +yAxis);
		_vertices[_vertexOffset + 3].quadrant.Set(-xAxis, +yAxis);

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

	void RectangleRenderer::Flush()
	{
		// Early exit if no vertices to draw
		if (_vertexOffset == 0)
			return;

		_shader.Bind();
		_shader.SetUniformMatrix3("uView", _view);

		_vertexArray.Bind();

		_vertexBuffer.Bind();
		_vertexBuffer.SetData(sizeof(RectangleRenderer) * _vertexOffset, _vertices.data());

		_indexBuffer.Bind();
		_indexBuffer.SetData(sizeof(unsigned int) * _indexOffset, _indices.data());

		glDrawElements(GL_TRIANGLES, _indexOffset, GL_UNSIGNED_INT, nullptr);

		_vertexOffset = 0;
		_indexOffset = 0;
	}
}
