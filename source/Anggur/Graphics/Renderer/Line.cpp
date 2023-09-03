#include "Anggur/Graphics/Renderer/Line.h"
#include "Anggur/Graphics/API.h"

namespace Anggur
{
	LineRenderer::LineRenderer()
	{
		_vertices.assign(_batchVertex, LineVertex{});

		_indices.assign(_batchVertex * _batchIndexMultiplier, 0);

		_vertexArray.Bind();
		_vertexArray.SetLayout({
			{VertexDataType::Float, 2, "aPosition"},
			{VertexDataType::Float, 2, "aPositionA"},
			{VertexDataType::Float, 2, "aPositionB"},
			{VertexDataType::Float, 2, "aQuadrant"},
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
		_shader.SetFragmentSource(R"(
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
		_shader.Compile();
	}

	void LineRenderer::SetView(const Matrix3& newView)
	{
		_view = newView;
	}

	void LineRenderer::Add(
		const Vector2& positionA, const Vector2& positionB, float thickness, float sharpness, const Vector4& color
	)
	{
		if (_vertexOffset + 4 > _vertices.size() || _indexOffset + 6 > _indices.size())
		{
			Flush();
		}

		Vector2 position(Math::Min(positionA.x, positionB.x), Math::Min(positionA.y, positionB.y));
		Vector2 size = Vector2(Math::Max(positionA.x, positionB.x), Math::Max(positionA.y, positionB.y)) - position;

		float edge = 0.5f * thickness + sharpness;
		float doubleSharpness = 2.0f * sharpness + thickness;
		float inverseMax = 1.0f / (Math::Max(size.x, size.y) + doubleSharpness);
		float xAxis = inverseMax * (size.x + doubleSharpness);
		float yAxis = inverseMax * (size.y + doubleSharpness);

		_vertices[_vertexOffset + 0].position = Vector2(position.x - edge, position.y - edge);
		_vertices[_vertexOffset + 1].position = Vector2(position.x + size.x + edge, position.y - edge);
		_vertices[_vertexOffset + 2].position = Vector2(position.x + size.x + edge, position.y + size.y + edge);
		_vertices[_vertexOffset + 3].position = Vector2(position.x - edge, position.y + size.y + edge);

		_vertices[_vertexOffset + 0].positionA = positionA - position;
		_vertices[_vertexOffset + 1].positionA = positionA - position;
		_vertices[_vertexOffset + 2].positionA = positionA - position;
		_vertices[_vertexOffset + 3].positionA = positionA - position;

		_vertices[_vertexOffset + 0].positionB = positionB - position;
		_vertices[_vertexOffset + 1].positionB = positionB - position;
		_vertices[_vertexOffset + 2].positionB = positionB - position;
		_vertices[_vertexOffset + 3].positionB = positionB - position;

		_vertices[_vertexOffset + 0].quadrant.Set(-xAxis, -yAxis);
		_vertices[_vertexOffset + 1].quadrant.Set(+xAxis, -yAxis);
		_vertices[_vertexOffset + 2].quadrant.Set(+xAxis, +yAxis);
		_vertices[_vertexOffset + 3].quadrant.Set(-xAxis, +yAxis);

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

	void LineRenderer::Flush()
	{
		// Early exit if no vertices to draw
		if (_vertexOffset == 0)
			return;

		_shader.Bind();
		_shader.SetUniformMatrix3("uView", _view);

		_vertexArray.Bind();

		_vertexBuffer.Bind();
		_vertexBuffer.SetData(sizeof(LineVertex) * _vertexOffset, _vertices.data());

		_indexBuffer.Bind();
		_indexBuffer.SetData(sizeof(unsigned int) * _indexOffset, _indices.data());

		glDrawElements(GL_TRIANGLES, _indexOffset, GL_UNSIGNED_INT, nullptr);

		_vertexOffset = 0;
		_indexOffset = 0;
	}
}
