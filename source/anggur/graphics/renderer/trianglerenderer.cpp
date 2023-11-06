#include "anggur/graphics/renderer/trianglerenderer.h"
#include "anggur/graphics/api.h"

namespace Anggur {
	TriangleRenderer::TriangleRenderer() {
		vertices.assign(batchVertex, TriangleVertex{});

		indices.assign(batchVertex * batchIndexMultiplier, 0);

		textures.assign(TextureSpecification::getMaxSlot(), nullptr);
		textureIndices.reserve(TextureSpecification::getMaxSlot());

		for (size_t i = 0; i < TextureSpecification::getMaxSlot(); ++i) {
			textureIndices.push_back(i);
		}

		vertexArray.bind();
		vertexArray.setLayout({
			{VertexDataType::Float, 2, "aPosition"},
			{VertexDataType::Float, 4, "aColor"},
			{VertexDataType::Float, 1, "aTextureIndex"},
			{VertexDataType::Float, 2, "aTexturePosition"},
		});

		vertexBuffer.bind();
		vertexBuffer.setCapacity(sizeof(TriangleVertex) * vertices.size());

		indexBuffer.bind();
		indexBuffer.setCapacity(sizeof(unsigned int) * indices.size());

		shader.bind();
		shader.setVertexSource(R"(
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
		shader.setFragmentSource(R"(
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
		shader.compile();
	}

	void TriangleRenderer::setView(const Matrix3& view) {
		this->view = view;
	}

	void TriangleRenderer::addTriangle(
		const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color,
		Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
		const Vector2& texturePosition2
	) {
		if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > vertices.size() ||
			textureOffset + 1 > textures.size()) {
			flush();
		}

		size_t textureIndex = 0;

		if (textureIndexMap.count(texture->getID())) {
			textureIndex = textureIndexMap[texture->getID()];
		} else {
			textureIndex = textureOffset;
			textureIndexMap[texture->getID()] = textureIndex;
			textures[textureIndex] = texture;

			++textureOffset;
		}

		vertices[vertexOffset + 0].position = position0;
		vertices[vertexOffset + 1].position = position1;
		vertices[vertexOffset + 2].position = position2;

		vertices[vertexOffset + 0].color = color;
		vertices[vertexOffset + 1].color = color;
		vertices[vertexOffset + 2].color = color;

		vertices[vertexOffset + 0].textureIndex = textureIndex;
		vertices[vertexOffset + 1].textureIndex = textureIndex;
		vertices[vertexOffset + 2].textureIndex = textureIndex;

		vertices[vertexOffset + 0].texturePosition = texturePosition0;
		vertices[vertexOffset + 1].texturePosition = texturePosition1;
		vertices[vertexOffset + 2].texturePosition = texturePosition2;

		indices[indexOffset + 0] = vertexOffset + 0;
		indices[indexOffset + 1] = vertexOffset + 1;
		indices[indexOffset + 2] = vertexOffset + 2;

		vertexOffset += 3;

		indexOffset += 3;
	}

	void TriangleRenderer::addQuad(
		const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3,
		const Vector4& color, Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
		const Vector2& texturePosition2, const Vector2& texturePosition3
	) {
		if (vertexOffset + 4 > vertices.size() || indexOffset + 6 > vertices.size() ||
			textureOffset + 1 > textures.size()) {
			flush();
		}

		size_t textureIndex = 0;

		if (textureIndexMap.count(texture->getID())) {
			textureIndex = textureIndexMap[texture->getID()];
		} else {
			textureIndex = textureOffset;
			textureIndexMap[texture->getID()] = textureIndex;
			textures[textureIndex] = texture;

			++textureOffset;
		}

		vertices[vertexOffset + 0].position = position0;
		vertices[vertexOffset + 1].position = position1;
		vertices[vertexOffset + 2].position = position2;
		vertices[vertexOffset + 3].position = position3;

		vertices[vertexOffset + 0].color = color;
		vertices[vertexOffset + 1].color = color;
		vertices[vertexOffset + 2].color = color;
		vertices[vertexOffset + 3].color = color;

		vertices[vertexOffset + 0].textureIndex = textureIndex;
		vertices[vertexOffset + 1].textureIndex = textureIndex;
		vertices[vertexOffset + 2].textureIndex = textureIndex;
		vertices[vertexOffset + 3].textureIndex = textureIndex;

		vertices[vertexOffset + 0].texturePosition = texturePosition0;
		vertices[vertexOffset + 1].texturePosition = texturePosition1;
		vertices[vertexOffset + 2].texturePosition = texturePosition2;
		vertices[vertexOffset + 3].texturePosition = texturePosition3;

		indices[indexOffset + 0] = vertexOffset + 0;
		indices[indexOffset + 1] = vertexOffset + 1;
		indices[indexOffset + 2] = vertexOffset + 2;
		indices[indexOffset + 3] = vertexOffset + 2;
		indices[indexOffset + 4] = vertexOffset + 3;
		indices[indexOffset + 5] = vertexOffset + 0;

		vertexOffset += 4;

		indexOffset += 6;
	}

	void TriangleRenderer::addRectangle(
		const Vector2& position, const Vector2& size, const Vector4& color, Texture* texture,
		const Vector2& texturePosition, const Vector2& textureSize
	) {
		addQuad(
			Vector2(position.x, position.y), Vector2(position.x + size.x, position.y),
			Vector2(position.x + size.x, position.y + size.y), Vector2(position.x, position.y + size.y), color, texture,
			Vector2(texturePosition.x, texturePosition.y),
			Vector2(texturePosition.x + textureSize.x, texturePosition.y),
			Vector2(texturePosition.x + textureSize.x, texturePosition.y + textureSize.y),
			Vector2(texturePosition.x, texturePosition.y + textureSize.y)
		);
	}

	void TriangleRenderer::flush() {
		// Early exit if no vertices to draw
		if (vertexOffset == 0)
			return;

		for (size_t textureIndex = 0; textureIndex < textureOffset; ++textureIndex)
			textures[textureIndex]->bind(textureIndex);

		shader.bind();
		shader.setUniformMatrix3("uView", view);
		shader.setUniformInt("uTextures", textureOffset, textureIndices.data());

		vertexArray.bind();

		vertexBuffer.bind();
		vertexBuffer.setData(sizeof(TriangleVertex) * vertexOffset, vertices.data());

		indexBuffer.bind();
		indexBuffer.setData(sizeof(unsigned int) * indexOffset, indices.data());

		glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

		vertexOffset = 0;
		indexOffset = 0;
		textureOffset = 0;

		textureIndexMap.clear();
	}
}
