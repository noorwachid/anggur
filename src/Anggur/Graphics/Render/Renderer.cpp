#include "Anggur/Core/Instrumentation.h"
#include "Anggur/Graphics/Function.h"
#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Graphics/Render/Renderer.h"
#include "Anggur/Math/Vector2.h"
#include <iostream>
#include <vector>

namespace Anggur
{
	Renderer::Renderer()
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		type = RendererType::Circle;

		std::vector<unsigned char> pixels = { 0xFF, 0xFF, 0xFF };
		defaultTexture = new Texture2D(pixels, 1, 1, 3);
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Flush()
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		switch (type) 
		{
			case RendererType::Mesh:
				return meshRenderer.Flush();

			case RendererType::Circle:
				return circleRenderer.Flush();

			case RendererType::RoundRectangle:
				return roundRectangleRenderer.Flush();

			case RendererType::Line:
				return lineRenderer.Flush();

			case RendererType::Text:
				return textRenderer.Flush();
		}

		++drawCount;
	}

	void Renderer::Clear()
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Renderer::SetClearColor(const Vector4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
	}


	void Renderer::SetViewport(const Vector2& position, const Vector2& size)
	{
		glViewport(position.x, position.y, size.x, size.y);
	}


	void Renderer::SetView(const Matrix3& newView)
	{
		meshRenderer.SetView(newView);
		circleRenderer.SetView(newView);
		roundRectangleRenderer.SetView(newView);
		lineRenderer.SetView(newView);
		textRenderer.SetView(newView);
	}

	void Renderer::BeginScene()
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

		Clear();

		drawCount = 0;
	}

	void Renderer::EndScene()
	{
		Flush();
	}

	void Renderer::BeginMask()
	{
		Flush();

		if (stencilDepth == 0)
			glEnable(GL_STENCIL_TEST);

		++stencilDepth;
	}

	void Renderer::BeginWriteMask()
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_ALWAYS, stencilDepth, stencilDepth);
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	}

	void Renderer::EndWriteMask()
	{
		Flush();

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilFunc(GL_EQUAL, stencilDepth, stencilDepth);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}

	void Renderer::BeginEraseMask()
	{
		Flush();

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_ALWAYS, stencilDepth, stencilDepth);
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

		--stencilDepth;

		if (stencilDepth == 0)
			glDisable(GL_STENCIL_TEST);
	}

	void Renderer::SetType(RendererType newType)
	{
		if (type != newType)
		{
			Flush();
			type = newType;
		}
	}

	void Renderer::DrawTriangle(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Mesh);

		meshRenderer.AddTriangle(position0, position1, position2, color, defaultTexture, Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f));
	}

	void Renderer::DrawTriangleTexture(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color, Texture2D* texture, const Vector2& texturePosition0, const Vector2& texturePosition1, const Vector2& texturePosition2)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Mesh);

		meshRenderer.AddTriangle(position0, position1, position2, color, texture, texturePosition0, texturePosition1, texturePosition2);
	}

	void Renderer::DrawQuad(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3, const Vector4& color)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Mesh);

		meshRenderer.AddQuad(position0, position1, position2, position3, color, defaultTexture, Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector2(0.0f, 1.0f));
	}

	void Renderer::DrawQuadTexture(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3, const Vector4& color, Texture2D* texture, const Vector2& texturePosition0, const Vector2& texturePosition1, const Vector2& texturePosition2, const Vector2& texturePosition3)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Mesh);

		meshRenderer.AddQuad(position0, position1, position2, position3, color, texture, texturePosition0, texturePosition1, texturePosition2, texturePosition3);
	}

	void Renderer::DrawRectangle(const Vector2& position, const Vector2& size, const Vector4& color)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Mesh);

		meshRenderer.AddRectangle(position, size, color, defaultTexture, Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f));
	}

	void Renderer::DrawRectangleTexture(const Vector2& position, const Vector2& size, const Vector4& color, Texture2D* texture, const Vector2& texturePosition, const Vector2& textureSize)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Mesh);

		meshRenderer.AddRectangle(position, size, color, texture, texturePosition, textureSize);
	}


	void Renderer::DrawCircle(const Vector2& position, float radius, float thickness, float sharpness, const Vector4& color)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Circle);

		circleRenderer.Add(position, radius, thickness, sharpness, color);
	}

	void Renderer::DrawLine(const Vector2& positionA, const Vector2& positionB, float thickness, float sharpness, const Vector4& color)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Line);

		lineRenderer.Add(positionA, positionB, thickness, sharpness, color);
	}

	void Renderer::DrawRoundRectangle(const Vector2& position, const Vector2& size, float radius, float thickness, float sharpness, const Vector4& color)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::RoundRectangle);

		roundRectangleRenderer.Add(position, size, radius, thickness, sharpness, color);
	}

	void Renderer::DrawText(const Vector2& position, unsigned int character, Font* font, float size, float thickness, float sharpness, const Vector4& color)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Text);

		textRenderer.Add(position, character, font, size, thickness, sharpness, color);
	}

	void Renderer::DrawText(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, const Vector4& color)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Text);

		textRenderer.Add(position, content, font, size, thickness, sharpness, color);
	}

	Vector2 Renderer::MeasureText(const std::string& content, Font* font, float size, float thickness, float sharpness)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		return textRenderer.Measure(content, font, size, thickness, sharpness);
	}

	void Renderer::DrawTextLine(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, const Vector4& color)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Text);

		textRenderer.AddLine(position, content, font, size, thickness, sharpness, color);
	}

	void Renderer::DrawTextLineCut(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, float limit, const Vector4& color)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Text);

		textRenderer.AddLineCut(position, content, font, size, thickness, sharpness, limit, color);
	}

	void Renderer::DrawTextFlow(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, float limit, const Vector4& color)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Text);

		textRenderer.AddFlow(position, content, font, size, thickness, sharpness, limit, color);
	}

	void Renderer::DrawTextFlowCut(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, const Vector2& limit, const Vector4& color)
	{
		ANGGUR_INSTRUMENTATION_PROFILE_FUNCTION;

		SetType(RendererType::Text);

		textRenderer.AddFlowCut(position, content, font, size, thickness, sharpness, limit, color);
	}
}
