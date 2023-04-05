#pragma once

#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Graphics/Render/CirclePipeline.h"
#include "Anggur/Graphics/Render/RRPipeline.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include <memory>
#include <vector>

namespace Anggur
{
	enum class PipelineType
	{
		Circle,
		RR,
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

		void BeginScene(const Matrix3& newView);
		void EndScene();

		void BeginMask();
		void BeginWriteMask();
		void EndWriteMask();
		void BeginEraseMask();
		void EndEraseMask();
		void EndMask();

		void AddCircle(const Vector2& position, float radius, float thickness, float sharpness, const Vector4& color);

		void AddRR(const Vector2& position, const Vector2& size, float radius, float thickness, float sharpness, const Vector4& color);

	private:
		void SetPipeline(PipelineType type);
		void SetView();

		uchar stencilDepth;

		Matrix3 view;

		PipelineType pipelineType;
		CirclePipeline circlePipeline;
		RRPipeline rrPipeline;
	};
}
