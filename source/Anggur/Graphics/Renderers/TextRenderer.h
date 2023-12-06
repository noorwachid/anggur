#pragma once

#include "Anggur/Graphics/Font.h"
#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include "Anggur/TextEncoding.h"

namespace Anggur
{
	struct TextVertex
	{
		Vector2 position;
		Vector2 texturePosition;
		float textureIndex = 0.0f;
		float thickness = 0.5f;
		float sharpness = 0.01f;
		float scale = 1.0f;
		Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	};

	class TextRenderer
	{
	public:
		TextRenderer();

		void SetView(const Matrix3& view);

		// +--
		// | H
		//
		void Add(
			const Vector2& position, unsigned int character, Font* font, float size, float thickness, float sharpness,
			const Vector4& color
		);

		// +----------------------------------
		// | Hello darkness my old friend.
		// | I've come to talk with you again.
		//
		void Add(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, const Vector4& color
		);

		Vector2 Measure(const std::string& content, Font* font, float size, float thickness, float sharpness);

		// +----------------------------------------------------------------
		// | Hello darkness my old friend. I've come to talk with you again.
		// +----------------------------------------------------------------
		//
		void AddLine(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, const Vector4& color
		);

		// +-----------------+
		// | Hello darkne... |
		// +-----------------+
		//
		void AddLineCut(
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
		void AddFlow(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, float limit, const Vector4& color
		);

		Vector2 MeasureFlow(
			const std::string& content, Font* font, float size, float thickness, float sharpness, float limit
		);

		// +-----------------+
		// | Hello darkness  |
		// | my old friend.  |
		// | I've come to... |
		// +-----------------+
		//
		void AddFlowCut(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, const Vector2& limit, const Vector4& color
		);

		void AddCharacter(
			const Vector2& position, const Vector2& size, float thickness, float sharpness, float scale,
			const Vector4& color, Texture* texture, const Vector2& texturePosition, const Vector2& textureSize
		);

		void Flush();

	private:
		void AddPartialLineCut(
			const Vector2& position, const std::string& content, size_t contentOffset, Font* font, float size,
			float thickness, float sharpness, float limit, const Vector4& color
		);

	private:
		VertexArray _vertexArray;
		VertexBuffer _vertexBuffer;
		IndexBuffer _indexBuffer;

		Shader _shader;

		Matrix3 _view;

		std::vector<TextVertex> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<Texture*> _textures;
		std::vector<int> _textureIndices;

		std::unordered_map<unsigned int, size_t> _textureIndexMap;

		size_t _vertexOffset = 0;
		size_t _indexOffset = 0;
		size_t _textureOffset = 0;

		size_t _drawCount = 0;

		size_t _batchVertex = 512;
		size_t _batchIndexMultiplier = 2;
	};
}
