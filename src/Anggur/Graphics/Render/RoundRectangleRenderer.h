#pragma once

#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Graphics/Render/Font.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include "Anggur/Graphics/Function.h"

namespace Anggur
{
	struct RRVertex 
	{
		Vector2 position;
		Vector2 size;
		Vector2 quadrant;
		float radius = 0.0f;
		float thickness = 0.5f;
		float sharpness = 0.01f;
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class RoundRectangleRenderer
	{
	public:
		RoundRectangleRenderer(); 

		void SetView(const Matrix3& newView);

		void Add(const Vector2& position, const Vector2& size, float radius, float thickness, float sharpness, const Vector4& color);

		void Flush();

	private:
		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		Shader shader;

		Matrix3 view;

		std::vector<RRVertex> vertices;
		std::vector<unsigned int> indices;

		size_t vertexOffset = 0;
		size_t indexOffset = 0;

		size_t drawCount = 0;

		size_t batchVertex = 512;
		size_t batchIndexMultiplier = 2;
	};
}

