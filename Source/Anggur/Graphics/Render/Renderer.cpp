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
		pipelineType = PipelineType::Circle;
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Draw()
	{
		switch (pipelineType) 
		{
			case PipelineType::Circle:
				circlePipeline.Draw();
				break;

			case PipelineType::RR:
				rrPipeline.Draw();
				break;
		}
	}

	void Renderer::Clear()
	{
		int flag = GL_COLOR_BUFFER_BIT | (stencilDepth ? GL_STENCIL_BUFFER_BIT : 0);
		glClear(flag);
	}

	void Renderer::SetClearColor(const Vector4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
	}


	void Renderer::SetViewport(const Vector2& position, const Vector2& size)
	{
		glViewport(position.x, position.y, size.x, size.y);
	}


	void Renderer::BeginScene(const Matrix3& newView)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

		view = newView;

		SetView();
		Clear();
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

	void Renderer::SetPipeline(PipelineType type)
	{
		if (pipelineType != type)
		{
			Draw();
			pipelineType = type;
			SetView();
		}
	}

	void Renderer::SetView()
	{
		switch (pipelineType) 
		{
			case PipelineType::Circle:
				circlePipeline.SetViewProjection(view);
				break;

			case PipelineType::RR:
				rrPipeline.SetView(view);
				break;
		}
	}

	void Renderer::AddCircle(const Vector2& position, float radius, float thickness, float sharpness, const Vector4& color)
	{
		SetPipeline(PipelineType::Circle);

		circlePipeline.Add(position, radius, thickness, sharpness, color);
	}

	void Renderer::AddRR(const Vector2& position, const Vector2& size, float radius, float thickness, float sharpness, const Vector4& color)
	{
		SetPipeline(PipelineType::RR);

		rrPipeline.Add(position, size, radius, thickness, sharpness, color);
	}
}
