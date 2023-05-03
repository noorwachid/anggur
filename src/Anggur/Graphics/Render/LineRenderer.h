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
	struct LineVertex 
	{
		Vector2 position;
		Vector2 positionA;
		Vector2 positionB;
		Vector2 quadrant;
		float thickness = 0.5f;
		float sharpness = 0.01f;
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class LineRenderer
	{
	public:
		LineRenderer(); 

		void SetView(const Matrix3& newView);

		void Add(const Vector2& positionA, const Vector2& positionB, float thickness, float sharpness, const Vector4& color);

		void Draw();

	private:
		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		Shader shader;

		Matrix3 view;

		std::vector<LineVertex> vertices;
		std::vector<uint> indices;

		usize vertexOffset = 0;
		usize indexOffset = 0;

		usize drawCount = 0;

		usize batchVertex = 512;
		usize batchIndexMultiplier = 2;
	};
}


