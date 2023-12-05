#include "anggur/graphics/renderer.h"
#include "anggur/graphics/api.h"
#include "anggur/graphics/shader.h"
#include "anggur/graphics/vertexbuffer.h"
#include "anggur/math/vector2.h"
#include <iostream>
#include <vector>

namespace Anggur
{
	Renderer::Renderer()
	{
		_type = RendererType::Circle;

		std::vector<unsigned char> pixels = {0xFF, 0xFF, 0xFF};
		_defaultTexture = new Texture(pixels, 1, 1, 3);
	}

	Renderer::~Renderer()
	{
		delete _defaultTexture;
	}

	void Renderer::Flush()
	{
		switch (_type)
		{
			case RendererType::Triangle:
				return _triangleRenderer.Flush();

			case RendererType::Circle:
				return _circleRenderer.Flush();

			case RendererType::RoundedRectangle:
				return _rectangleRenderer.Flush();

			case RendererType::Line:
				return _lineRenderer.Flush();

			case RendererType::Text:
				return _textRenderer.Flush();
		}

		++_drawCount;
	}

	void Renderer::Clear(const Vector4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Renderer::SetViewport(const Vector2& position, const Vector2& size)
	{
		glViewport(position.x, position.y, size.x, size.y);
	}

	void Renderer::SetView(const Matrix3& newView)
	{
		_triangleRenderer.SetView(newView);
		_rectangleRenderer.SetView(newView);
		_circleRenderer.SetView(newView);
		_lineRenderer.SetView(newView);
		_textRenderer.SetView(newView);
	}

	void Renderer::BeginScene()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		_drawCount = 0;
	}

	void Renderer::EndScene()
	{
		Flush();
	}

	void Renderer::BeginMask()
	{
		Flush();

		if (_stencilDepth == 0)
			glEnable(GL_STENCIL_TEST);

		++_stencilDepth;
	}

	void Renderer::BeginWriteMask()
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_ALWAYS, _stencilDepth, _stencilDepth);
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	}

	void Renderer::EndWriteMask()
	{
		Flush();

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilFunc(GL_EQUAL, _stencilDepth, _stencilDepth);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}

	void Renderer::BeginEraseMask()
	{
		Flush();

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_ALWAYS, _stencilDepth, _stencilDepth);
		glStencilOp(GL_DECR, GL_DECR, GL_DECR);
	}

	void Renderer::EndEraseMask()
	{
		Flush();

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
	}

	void Renderer::EndMask()
	{
		Flush();

		--_stencilDepth;

		if (_stencilDepth == 0)
			glDisable(GL_STENCIL_TEST);
	}

	void Renderer::SetType(RendererType newType)
	{
		if (_type != newType)
		{
			Flush();
			_type = newType;
		}
	}

	void Renderer::DrawTriangle(
		const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color
	)
	{
		SetType(RendererType::Triangle);

		_triangleRenderer.AddTriangle(
			position0, position1, position2, color, _defaultTexture, Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f),
			Vector2(1.0f, 1.0f)
		);
	}

	void Renderer::DrawTriangleTexture(
		const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color,
		Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
		const Vector2& texturePosition2
	)
	{
		SetType(RendererType::Triangle);

		_triangleRenderer.AddTriangle(
			position0, position1, position2, color, texture, texturePosition0, texturePosition1, texturePosition2
		);
	}

	void Renderer::DrawQuad(
		const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3,
		const Vector4& color
	)
	{
		SetType(RendererType::Triangle);

		_triangleRenderer.AddQuad(
			position0, position1, position2, position3, color, _defaultTexture, Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f),
			Vector2(1.0f, 1.0f), Vector2(0.0f, 1.0f)
		);
	}

	void Renderer::DrawQuadTexture(
		const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3,
		const Vector4& color, Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
		const Vector2& texturePosition2, const Vector2& texturePosition3
	)
	{
		SetType(RendererType::Triangle);

		_triangleRenderer.AddQuad(
			position0, position1, position2, position3, color, texture, texturePosition0, texturePosition1,
			texturePosition2, texturePosition3
		);
	}

	void Renderer::DrawRectangle(const Vector2& position, const Vector2& size, const Vector4& color)
	{
		SetType(RendererType::Triangle);

		_triangleRenderer.AddRectangle(position, size, color, _defaultTexture, Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f));
	}

	void Renderer::DrawRectangleTexture(
		const Vector2& position, const Vector2& size, const Vector4& color, Texture* texture,
		const Vector2& texturePosition, const Vector2& textureSize
	)
	{
		SetType(RendererType::Triangle);

		_triangleRenderer.AddRectangle(position, size, color, texture, texturePosition, textureSize);
	}

	void Renderer::DrawRoundedRectangle(
		const Vector2& position, const Vector2& size, float radius, float thickness, float sharpness,
		const Vector4& color
	)
	{
		SetType(RendererType::RoundedRectangle);

		_rectangleRenderer.Add(position, size, radius, thickness, sharpness, color);
	}

	void Renderer::DrawCircle(
		const Vector2& position, float radius, float thickness, float sharpness, const Vector4& color
	)
	{
		SetType(RendererType::Circle);

		_circleRenderer.Add(position, radius, thickness, sharpness, color);
	}

	void Renderer::DrawLine(
		const Vector2& positionA, const Vector2& positionB, float thickness, float sharpness, const Vector4& color
	)
	{
		SetType(RendererType::Line);

		_lineRenderer.Add(positionA, positionB, thickness, sharpness, color);
	}

	void Renderer::DrawCharacter(
		const Vector2& position, unsigned int character, Font* font, float size, float thickness, float sharpness,
		const Vector4& color
	)
	{
		SetType(RendererType::Text);

		_textRenderer.Add(position, character, font, size, thickness, sharpness, color);
	}

	void Renderer::DrawText(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		const Vector4& color
	)
	{
		SetType(RendererType::Text);

		_textRenderer.Add(position, content, font, size, thickness, sharpness, color);
	}

	void Renderer::DrawTextLine(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		const Vector4& color
	)
	{
		SetType(RendererType::Text);

		_textRenderer.AddLine(position, content, font, size, thickness, sharpness, color);
	}

	void Renderer::DrawTextLineCut(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		float limit, const Vector4& color
	)
	{
		SetType(RendererType::Text);

		_textRenderer.AddLineCut(position, content, font, size, thickness, sharpness, limit, color);
	}

	void Renderer::DrawTextFlow(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		float limit, const Vector4& color
	)
	{
		SetType(RendererType::Text);

		_textRenderer.AddFlow(position, content, font, size, thickness, sharpness, limit, color);
	}

	void Renderer::DrawTextFlowCut(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		const Vector2& limit, const Vector4& color
	)
	{
		SetType(RendererType::Text);

		_textRenderer.AddFlowCut(position, content, font, size, thickness, sharpness, limit, color);
	}

	Vector2 Renderer::MeasureText(const std::string& content, Font* font, float size, float thickness, float sharpness)
	{
		return _textRenderer.Measure(content, font, size, thickness, sharpness);
	}

	Vector2 Renderer::MeasureTextFlow(
		const std::string& content, Font* font, float size, float thickness, float sharpness, float limit
	)
	{
		return _textRenderer.MeasureFlow(content, font, size, thickness, sharpness, limit);
	}
}
