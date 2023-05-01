#pragma once

#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Graphics/Render/Font.h"
#include "Anggur/Graphics/Function.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include "Anggur/Utility/UTF8.h"

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
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class TextRenderer
	{
	public:
		TextRenderer();

		void SetView(const Matrix3& newView);

		// +----------------------------------
		// | Hello darkness my old friend.   
		// | I've come to talk with you again. 
		// 
		void Add(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, const Vector4& color);


		// +----------------------------------------------------------------
		// | Hello darkness my old friend. I've come to talk with you again. 
		// +----------------------------------------------------------------
		//
		void AddLine(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, const Vector4& color);

		// +-----------------+
		// | Hello darkne... |
		// +-----------------+
		//
		void AddLineCut(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, float limit, const Vector4& color);

		// +-----------------+
		// | Hello darkness  |
		// | my old friend.  |
		// | I've come to    |
		// | talk with you   |
		// | again.          |
		// +-----------------+
		//
		void AddFlow(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, float limit, const Vector4& color);

		// +-----------------+
		// | Hello darkness  |
		// | my old friend.  |
		// | I've come to... |
		// +-----------------+
		//
		void AddFlowCut(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, const Vector2& limit, const Vector4& color);

		void AddCharacter(const Vector2& position, const Vector2& size, float thickness, float sharpness, float scale, const Vector4& color, Texture2D* texture, const Vector2& texturePosition, const Vector2& textureSize);

		void Draw();

	private:
		void AddPartialLineCut(const Vector2& position, const std::string& content, usize contentOffset, Font* font, float size, float thickness, float sharpness, float limit, const Vector4& color);

	private:
		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		Shader shader;

		Matrix3 view;

		std::vector<TextVertex> vertices;
		std::vector<uint> indices;
		std::vector<Texture2D*> textures;
		std::vector<int> textureIndices;

		std::unordered_map<uint, usize> textureIndexMap;

		usize vertexOffset = 0;
		usize indexOffset = 0;
		usize textureOffset = 0;

		usize drawCount = 0;

		usize batchVertex = 512;
		usize batchIndexMultiplier = 2;
	};
}


