#pragma once

#include "anggur/graphics/font.h"
#include "anggur/graphics/shader.h"
#include "anggur/graphics/texture.h"
#include "anggur/graphics/vertexbuffer.h"
#include "anggur/math/matrix3.h"
#include "anggur/math/vector2.h"
#include "anggur/math/vector4.h"
#include "anggur/textencoding.h"

namespace Anggur {
	struct TextVertex {
		Vector2 position;
		Vector2 texturePosition;
		float textureIndex = 0.0f;
		float thickness = 0.5f;
		float sharpness = 0.01f;
		float scale = 1.0f;
		Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	};

	class TextRenderer {
	public:
		TextRenderer();

		void setView(const Matrix3& view);

		// +--
		// | H
		//
		void add(
			const Vector2& position, unsigned int character, Font* font, float size, float thickness, float sharpness,
			const Vector4& color
		);

		// +----------------------------------
		// | Hello darkness my old friend.
		// | I've come to talk with you again.
		//
		void add(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, const Vector4& color
		);

		Vector2 Measure(const std::string& content, Font* font, float size, float thickness, float sharpness);

		// +----------------------------------------------------------------
		// | Hello darkness my old friend. I've come to talk with you again.
		// +----------------------------------------------------------------
		//
		void addLine(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, const Vector4& color
		);

		// +-----------------+
		// | Hello darkne... |
		// +-----------------+
		//
		void addLineCut(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, float limit, const Vector4& color
		);

		// +-----------------+
		// | Hello darkness  |
		// | my old friend.  |
		// | I've come to    |
		// | talk with you   |
		// | again.          |
		// +-----------------+
		//
		void addFlow(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, float limit, const Vector4& color
		);

		// +-----------------+
		// | Hello darkness  |
		// | my old friend.  |
		// | I've come to... |
		// +-----------------+
		//
		void addFlowCut(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, const Vector2& limit, const Vector4& color
		);

		void addCharacter(
			const Vector2& position, const Vector2& size, float thickness, float sharpness, float scale,
			const Vector4& color, Texture* texture, const Vector2& texturePosition, const Vector2& textureSize
		);

		void flush();

	private:
		void addPartialLineCut(
			const Vector2& position, const std::string& content, size_t contentOffset, Font* font, float size,
			float thickness, float sharpness, float limit, const Vector4& color
		);

	private:
		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		Shader shader;

		Matrix3 view;

		std::vector<TextVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture*> textures;
		std::vector<int> textureIndices;

		std::unordered_map<unsigned int, size_t> textureIndexMap;

		size_t vertexOffset = 0;
		size_t indexOffset = 0;
		size_t textureOffset = 0;

		size_t drawCount = 0;

		size_t batchVertex = 512;
		size_t batchIndexMultiplier = 2;
	};
}
