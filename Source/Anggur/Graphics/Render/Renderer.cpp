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
		type = RendererType::Circle;

		std::vector<uchar> pixels = { 0xFF, 0xFF, 0xFF };
		defaultTexture = new Texture2D(pixels, 1, 1, 3);
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Draw()
	{
		switch (type) 
		{
			case RendererType::Mesh:
				return meshRenderer.Draw();

			case RendererType::Circle:
				return circleRenderer.Draw();

			case RendererType::RoundRectangle:
				return roundRectangleRenderer.Draw();

			case RendererType::Line:
				return lineRenderer.Draw();

			case RendererType::Text:
				return textRenderer.Draw();
		}

		++drawCount;
	}

	void Renderer::Clear()
	{
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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

		Clear();

		drawCount = 0;
	}

	void Renderer::EndScene()
	{
		Draw();
	}

	void Renderer::BeginMask()
	{
		Draw();

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
		Draw();

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilFunc(GL_EQUAL, stencilDepth, stencilDepth);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}

	void Renderer::BeginEraseMask()
	{
		Draw();

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_ALWAYS, stencilDepth, stencilDepth);
		glStencilOp(GL_DECR, GL_DECR, GL_DECR);
	}

	void Renderer::EndEraseMask()
	{
		Draw();

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
	}

	void Renderer::EndMask()
	{
		Draw();

		--stencilDepth;

		if (stencilDepth == 0)
			glDisable(GL_STENCIL_TEST);
	}

	void Renderer::SetType(RendererType newType)
	{
		if (type != newType)
		{
			Draw();
			type = newType;
		}
	}

	void Renderer::AddTriangle(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color)
	{
		SetType(RendererType::Mesh);

		meshRenderer.AddTriangle(position0, position1, position2, color, defaultTexture, Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f));
	}

	void Renderer::AddTriangleTexture(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color, Texture2D* texture, const Vector2& texturePosition0, const Vector2& texturePosition1, const Vector2& texturePosition2)
	{
		SetType(RendererType::Mesh);

		meshRenderer.AddTriangle(position0, position1, position2, color, texture, texturePosition0, texturePosition1, texturePosition2);
	}

	void Renderer::AddQuad(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3, const Vector4& color)
	{
		SetType(RendererType::Mesh);

		meshRenderer.AddQuad(position0, position1, position2, position3, color, defaultTexture, Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector2(0.0f, 1.0f));
	}

	void Renderer::AddQuadTexture(const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3, const Vector4& color, Texture2D* texture, const Vector2& texturePosition0, const Vector2& texturePosition1, const Vector2& texturePosition2, const Vector2& texturePosition3)
	{
		SetType(RendererType::Mesh);

		meshRenderer.AddQuad(position0, position1, position2, position3, color, texture, texturePosition0, texturePosition1, texturePosition2, texturePosition3);
	}

	void Renderer::AddRectangle(const Vector2& position, const Vector2& size, const Vector4& color)
	{
		SetType(RendererType::Mesh);

		meshRenderer.AddRectangle(position, size, color, defaultTexture, Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f));
	}

	void Renderer::AddRectangleTexture(const Vector2& position, const Vector2& size, const Vector4& color, Texture2D* texture, const Vector2& texturePosition, const Vector2& textureSize)
	{
		SetType(RendererType::Mesh);

		meshRenderer.AddRectangle(position, size, color, texture, texturePosition, textureSize);
	}


	void Renderer::AddCircle(const Vector2& position, float radius, float thickness, float sharpness, const Vector4& color)
	{
		SetType(RendererType::Circle);

		circleRenderer.Add(position, radius, thickness, sharpness, color);
	}

	void Renderer::AddLine(const Vector2& positionA, const Vector2& positionB, float thickness, float sharpness, const Vector4& color)
	{
		SetType(RendererType::Line);

		lineRenderer.Add(positionA, positionB, thickness, sharpness, color);
	}

	void Renderer::AddRoundRectangle(const Vector2& position, const Vector2& size, float radius, float thickness, float sharpness, const Vector4& color)
	{
		SetType(RendererType::RoundRectangle);

		roundRectangleRenderer.Add(position, size, radius, thickness, sharpness, color);
	}


	void Renderer::AddTextCharacter(const Vector2& position, const Vector2& size, float thickness, float sharpness, float scale, const Vector4& color, Texture2D* texture, const Vector2& texturePosition, const Vector2& textureSize)
	{
		SetType(RendererType::Text);

		textRenderer.AddCharacter(position, size, thickness, sharpness, scale, color, texture, texturePosition, textureSize);
	}

	void Renderer::AddTextLine(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, const Vector4& color)
	{
		SetType(RendererType::Text);

		textRenderer.AddLine(position, content, font, size, thickness, sharpness, color);
	}

	void Renderer::AddTextLines(const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness, const Vector4& color)
	{
		SetType(RendererType::Text);

		textRenderer.AddLines(position, content, font, size, thickness, sharpness, color);
	}
}
