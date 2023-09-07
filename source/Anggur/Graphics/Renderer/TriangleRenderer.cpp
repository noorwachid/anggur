#include "Anggur/Graphics/Renderer/TriangleRenderer.h"
#include "Anggur/Graphics/API.h"

namespace Anggur
{
	TriangleRenderer::TriangleRenderer()
	{
		_vertices.assign(_batchVertex, TriangleVertex{});

		_indices.assign(_batchVertex * _batchIndexMultiplier, 0);

		_textures.assign(TextureSpecification::GetMaxSlot(), nullptr);
		_textureIndices.reserve(TextureSpecification::GetMaxSlot());

		for (size_t i = 0; i < TextureSpecification::GetMaxSlot(); ++i)
		{
			_textureIndices.push_back(i);
		}

		_vertexArray.Bind();
		_vertexArray.SetLayout({
			{VertexDataType::Float, 2, "aPosition"},
			{VertexDataType::Float, 4, "aColor"},
			{VertexDataType::Float, 1, "aTextureIndex"},
			{VertexDataType::Float, 2, "aTexturePosition"},
		});

		_vertexBuffer.Bind();
		_vertexBuffer.SetCapacity(sizeof(TriangleVertex) * _vertices.size());

		_indexBuffer.Bind();
		_indexBuffer.SetCapacity(sizeof(unsigned int) * _indices.size());

		_shader.Bind();
		_shader.SetVertexSource(R"(
			layout (location = 0) in vec2 aPosition;
			layout (location = 1) in vec4 aColor;
			layout (location = 2) in float aTextureIndex;
			layout (location = 3) in vec2 aTexturePosition;

			out vec4 vColor;
			out float vTextureIndex;
			out vec2 vTexturePosition;

			uniform mat3 uView;

			void main() {
				gl_Position.xyz = uView * vec3(aPosition, 1.0f);
				gl_Position.w = 1.0f;

				vColor = aColor;
				vTextureIndex = aTextureIndex;
				vTexturePosition = aTexturePosition;
			}
		)");
		_shader.SetFragmentSource(R"(
			in vec4 vColor;
			in float vTextureIndex;
			in vec2 vTexturePosition;

			uniform sampler2D uTextures[TEXTURE_SLOT];

			out vec4 fColor;

			void main() 
			{
				int textureIndex = int(vTextureIndex);

				TEXTURE_SLOT_INDEXING(fColor, uTextures, textureIndex, vTexturePosition)

				fColor *= vColor;

				if (fColor.w == 0.0f) {
					discard;
				}
			}
		)");
		_shader.Compile();
	}

	void TriangleRenderer::SetView(const Matrix3& newView)
	{
		_view = newView;
	}

	void TriangleRenderer::AddTriangle(
		const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color,
		Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
		const Vector2& texturePosition2
	)
	{
		if (_vertexOffset + 4 > _vertices.size() || _indexOffset + 6 > _vertices.size() ||
			_textureOffset + 1 > _textures.size())
		{
			Flush();
		}

		size_t textureIndex = 0;

		if (_textureIndexMap.count(texture->GetID()))
		{
			textureIndex = _textureIndexMap[texture->GetID()];
		}
		else
		{
			textureIndex = _textureOffset;
			_textureIndexMap[texture->GetID()] = textureIndex;
			_textures[textureIndex] = texture;

			++_textureOffset;
		}

		_vertices[_vertexOffset + 0].position = position0;
		_vertices[_vertexOffset + 1].position = position1;
		_vertices[_vertexOffset + 2].position = position2;

		_vertices[_vertexOffset + 0].color = color;
		_vertices[_vertexOffset + 1].color = color;
		_vertices[_vertexOffset + 2].color = color;

		_vertices[_vertexOffset + 0].textureIndex = textureIndex;
		_vertices[_vertexOffset + 1].textureIndex = textureIndex;
		_vertices[_vertexOffset + 2].textureIndex = textureIndex;

		_vertices[_vertexOffset + 0].texturePosition = texturePosition0;
		_vertices[_vertexOffset + 1].texturePosition = texturePosition1;
		_vertices[_vertexOffset + 2].texturePosition = texturePosition2;

		_indices[_indexOffset + 0] = _vertexOffset + 0;
		_indices[_indexOffset + 1] = _vertexOffset + 1;
		_indices[_indexOffset + 2] = _vertexOffset + 2;

		_vertexOffset += 3;

		_indexOffset += 3;
	}

	void TriangleRenderer::AddQuad(
		const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3,
		const Vector4& color, Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
		const Vector2& texturePosition2, const Vector2& texturePosition3
	)
	{
		if (_vertexOffset + 4 > _vertices.size() || _indexOffset + 6 > _vertices.size() ||
			_textureOffset + 1 > _textures.size())
		{
			Flush();
		}

		size_t textureIndex = 0;

		if (_textureIndexMap.count(texture->GetID()))
		{
			textureIndex = _textureIndexMap[texture->GetID()];
		}
		else
		{
			textureIndex = _textureOffset;
			_textureIndexMap[texture->GetID()] = textureIndex;
			_textures[textureIndex] = texture;

			++_textureOffset;
		}

		_vertices[_vertexOffset + 0].position = position0;
		_vertices[_vertexOffset + 1].position = position1;
		_vertices[_vertexOffset + 2].position = position2;
		_vertices[_vertexOffset + 3].position = position3;

		_vertices[_vertexOffset + 0].color = color;
		_vertices[_vertexOffset + 1].color = color;
		_vertices[_vertexOffset + 2].color = color;
		_vertices[_vertexOffset + 3].color = color;

		_vertices[_vertexOffset + 0].textureIndex = textureIndex;
		_vertices[_vertexOffset + 1].textureIndex = textureIndex;
		_vertices[_vertexOffset + 2].textureIndex = textureIndex;
		_vertices[_vertexOffset + 3].textureIndex = textureIndex;

		_vertices[_vertexOffset + 0].texturePosition = texturePosition0;
		_vertices[_vertexOffset + 1].texturePosition = texturePosition1;
		_vertices[_vertexOffset + 2].texturePosition = texturePosition2;
		_vertices[_vertexOffset + 3].texturePosition = texturePosition3;

		_indices[_indexOffset + 0] = _vertexOffset + 0;
		_indices[_indexOffset + 1] = _vertexOffset + 1;
		_indices[_indexOffset + 2] = _vertexOffset + 2;
		_indices[_indexOffset + 3] = _vertexOffset + 2;
		_indices[_indexOffset + 4] = _vertexOffset + 3;
		_indices[_indexOffset + 5] = _vertexOffset + 0;

		_vertexOffset += 4;

		_indexOffset += 6;
	}

	void TriangleRenderer::AddRectangle(
		const Vector2& position, const Vector2& size, const Vector4& color, Texture* texture,
		const Vector2& texturePosition, const Vector2& textureSize
	)
	{
		AddQuad(
			Vector2(position.x, position.y), Vector2(position.x + size.x, position.y),
			Vector2(position.x + size.x, position.y + size.y), Vector2(position.x, position.y + size.y), color, texture,
			Vector2(texturePosition.x, texturePosition.y),
			Vector2(texturePosition.x + textureSize.x, texturePosition.y),
			Vector2(texturePosition.x + textureSize.x, texturePosition.y + textureSize.y),
			Vector2(texturePosition.x, texturePosition.y + textureSize.y)
		);
	}

	void TriangleRenderer::Flush()
	{
		// Early exit if no vertices to draw
		if (_vertexOffset == 0)
			return;

		for (size_t textureIndex = 0; textureIndex < _textureOffset; ++textureIndex)
			_textures[textureIndex]->Bind(textureIndex);

		_shader.Bind();
		_shader.SetUniformMatrix3("uView", _view);
		_shader.SetUniformInt("uTextures", _textureOffset, _textureIndices.data());

		_vertexArray.Bind();

		_vertexBuffer.Bind();
		_vertexBuffer.SetData(sizeof(TriangleVertex) * _vertexOffset, _vertices.data());

		_indexBuffer.Bind();
		_indexBuffer.SetData(sizeof(unsigned int) * _indexOffset, _indices.data());

		glDrawElements(GL_TRIANGLES, _indexOffset, GL_UNSIGNED_INT, nullptr);

		_vertexOffset = 0;
		_indexOffset = 0;
		_textureOffset = 0;

		_textureIndexMap.clear();
	}
}
