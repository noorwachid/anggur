#pragma once

#include "Anggur/Graphics/Renderer/Circle.h"
#include "Anggur/Graphics/Renderer/Line.h"
#include "Anggur/Graphics/Renderer/Rectangle.h"
#include "Anggur/Graphics/Renderer/Text.h"
#include "Anggur/Graphics/Renderer/Triangle.h"
#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include <memory>
#include <vector>

namespace Anggur
{
	enum class RendererType
	{
		Triangle,
		Circle,
		// arc,
		Rectangle,
		Line,
		// spline,
		Text,
	};

	class Renderer
	{
	public:
		Renderer();

		~Renderer();

		void Flush();

		void ClearBackground(const Vector4& color = Vector4::black);
		void SetViewport(const Vector2& position, const Vector2& size);
		void SetView(const Matrix3& newView);

		void BeginScene();
		void EndScene();

		void BeginMask();
		void BeginWriteMask();
		void EndWriteMask();
		void BeginEraseMask();
		void EndEraseMask();
		void EndMask();

		void DrawTriangle(
			const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color
		);
		void DrawTriangleTexture(
			const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color,
			Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
			const Vector2& texturePosition2
		);

		void DrawQuad(
			const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3,
			const Vector4& color
		);
		void DrawQuadTexture(
			const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3,
			const Vector4& color, Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
			const Vector2& texturePosition2, const Vector2& texturePosition3
		);

		void DrawRectangle(const Vector2& position, const Vector2& size, const Vector4& color);
		void DrawRectangleTexture(
			const Vector2& position, const Vector2& size, const Vector4& color, Texture* texture,
			const Vector2& texturePosition, const Vector2& textureSize
		);

		void DrawRectangle(
			const Vector2& position, const Vector2& size, float radius, float thickness, float sharpness,
			const Vector4& color
		);

		void DrawCircle(const Vector2& position, float radius, float thickness, float sharpness, const Vector4& color);

		void DrawLine(
			const Vector2& positionA, const Vector2& positionB, float thickness, float sharpness, const Vector4& color
		);

		void DrawText(
			const Vector2& position, unsigned int character, Font* font, float size, float thickness, float sharpness,
			const Vector4& color
		);

		void DrawText(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, const Vector4& color
		);

		Vector2 MeasureText(const std::string& content, Font* font, float size, float thickness, float sharpness);

		void DrawTextLine(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, const Vector4& color
		);

		void DrawTextLineCut(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, float limit, const Vector4& color
		);

		void DrawTextFlow(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, float limit, const Vector4& color
		);

		void DrawTextFlowCut(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, const Vector2& limit, const Vector4& color
		);

	private:
		void SetType(RendererType type);

		unsigned char _stencilDepth = 0;

		size_t _drawCount = 0;

		RendererType _type;

		TriangleRenderer _triangleRenderer;
		CircleRenderer _circleRenderer;
		RectangleRenderer _rectangleRenderer;
		LineRenderer _lineRenderer;
		TextRenderer _textRenderer;

		Texture* _defaultTexture = nullptr;
	};
}
