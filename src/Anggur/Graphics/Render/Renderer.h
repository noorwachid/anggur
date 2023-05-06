#pragma once

#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Graphics/Render/MeshRenderer.h"
#include "Anggur/Graphics/Render/CircleRenderer.h"
#include "Anggur/Graphics/Render/LineRenderer.h"
#include "Anggur/Graphics/Render/RoundRectangleRenderer.h"
#include "Anggur/Graphics/Render/TextRenderer.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include <memory>
#include <vector>

namespace Anggur
{
	enum class RendererType
	{
		Mesh,
		Circle,
		// Arc,
		RoundRectangle,
		Line,
		// Spline,
		Text,
	};

	class Renderer
	{
	public:
		Renderer();

		~Renderer();

		void Draw();

		void Clear();

		void SetClearColor(const Vector4& color = Vector4::black);
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

		void AddTriangle(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color);
		void AddTriangleTexture(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color, Texture2D* texture, const Vector2& texturePosition0, const Vector2& texturePosition1, const Vector2& texturePosition2);

		void AddQuad(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3, const Vector4& color);
		void AddQuadTexture(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3, const Vector4& color, Texture2D* texture, const Vector2& texturePosition0, const Vector2& texturePosition1, const Vector2& texturePosition2, const Vector2& texturePosition3);

		void AddRectangle(const Vector2& position, const Vector2& size, const Vector4& color);
		void AddRectangleTexture(const Vector2& position, const Vector2& size, const Vector4& color, Texture2D* texture, const Vector2& texturePosition, const Vector2& textureSize);

		void AddCircle(const Vector2& position, float radius, float thickness, float sharpness, const Vector4& color);

		void AddLine(const Vector2& positionA, const Vector2& positionB, float thickness, float sharpness, const Vector4& color);

		void AddRoundRectangle(const Vector2& position, const Vector2& size, float radius, float thickness, float sharpness, const Vector4& color);

		void AddText(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, const Vector4& color);

		Vector2 MeasureText(const std::string& content, Font* font, float size, float thickness, float sharpness);

		void AddTextLine(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, const Vector4& color);

		void AddTextLineCut(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, float limit, const Vector4& color);

		void AddTextFlow(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, float limit, const Vector4& color);

		void AddTextFlowCut(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, const Vector2& limit, const Vector4& color);

	private:
		void SetType(RendererType type);

		uchar stencilDepth = 0;

		usize drawCount = 0;

		RendererType type;

		MeshRenderer meshRenderer;
		CircleRenderer circleRenderer;
		RoundRectangleRenderer roundRectangleRenderer;
		LineRenderer lineRenderer;
		TextRenderer textRenderer;

		Texture2D* defaultTexture = nullptr;
	};
}
