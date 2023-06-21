#pragma once

#include "Anggur/Graphics/Render/CircleRenderer.h"
#include "Anggur/Graphics/Render/LineRenderer.h"
#include "Anggur/Graphics/Render/MeshRenderer.h"
#include "Anggur/Graphics/Render/RoundRectangleRenderer.h"
#include "Anggur/Graphics/Render/TextRenderer.h"
#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include <memory>
#include <vector>

namespace Anggur {
	enum class RendererType {
		mesh,
		circle,
		// arc,
		roundRectangle,
		line,
		// spline,
		text,
	};

	class Renderer {
	public:
		Renderer();

		~Renderer();

		void flush();

		void clear();

		void setClearColor(const Vector4& color = Vector4::black);
		void setViewport(const Vector2& position, const Vector2& size);
		void setView(const Matrix3& newView);

		void beginScene();
		void endScene();

		void beginMask();
		void beginWriteMask();
		void endWriteMask();
		void beginEraseMask();
		void endEraseMask();
		void endMask();

		void drawTriangle(
			const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color
		);
		void drawTriangleTexture(
			const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color,
			Texture2D* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
			const Vector2& texturePosition2
		);

		void drawQuad(
			const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3,
			const Vector4& color
		);
		void drawQuadTexture(
			const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3,
			const Vector4& color, Texture2D* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
			const Vector2& texturePosition2, const Vector2& texturePosition3
		);

		void drawRectangle(const Vector2& position, const Vector2& size, const Vector4& color);
		void drawRectangleTexture(
			const Vector2& position, const Vector2& size, const Vector4& color, Texture2D* texture,
			const Vector2& texturePosition, const Vector2& textureSize
		);

		void drawCircle(const Vector2& position, float radius, float thickness, float sharpness, const Vector4& color);

		void drawLine(
			const Vector2& positionA, const Vector2& positionB, float thickness, float sharpness, const Vector4& color
		);

		void drawRoundRectangle(
			const Vector2& position, const Vector2& size, float radius, float thickness, float sharpness,
			const Vector4& color
		);

		void drawText(
			const Vector2& position, unsigned int character, Font* font, float size, float thickness, float sharpness,
			const Vector4& color
		);

		void drawText(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, const Vector4& color
		);

		Vector2 measureText(const std::string& content, Font* font, float size, float thickness, float sharpness);

		void drawTextLine(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, const Vector4& color
		);

		void drawTextLineCut(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, float limit, const Vector4& color
		);

		void drawTextFlow(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, float limit, const Vector4& color
		);

		void drawTextFlowCut(
			const Vector2& position, const std::string& content, Font* font, float size, float thickness,
			float sharpness, const Vector2& limit, const Vector4& color
		);

	private:
		void setType(RendererType type);

		unsigned char stencilDepth = 0;

		size_t drawCount = 0;

		RendererType type;

		MeshRenderer meshRenderer;
		CircleRenderer circleRenderer;
		RoundRectangleRenderer roundRectangleRenderer;
		LineRenderer lineRenderer;
		TextRenderer textRenderer;

		Texture2D* defaultTexture = nullptr;
	};
}
