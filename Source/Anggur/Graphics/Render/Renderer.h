#pragma once

#include "Anggur/Graphics/Shader.h"
#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/VertexBuffer.h"
#include "Anggur/Graphics/Render/MeshPipeline.h"
#include "Anggur/Graphics/Render/CirclePipeline.h"
#include "Anggur/Graphics/Render/RRPipeline.h"
#include "Anggur/Graphics/Render/TextPipeline.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include <memory>
#include <vector>

namespace Anggur
{
	enum class PipelineType
	{
		Mesh,
		Circle,
		// Arc,
		RR,
		// Line,
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

		void AddRectangle(const Vector2& position, const Vector2& size, const Vector4& color);
		void AddRectangleTexture(const Vector2& position, const Vector2& size, const Vector4& color, Texture2D* texture, const Vector2& texturePosition, const Vector2& textureSize);

		void AddCircle(const Vector2& position, float radius, float thickness, float sharpness, const Vector4& color);

		void AddRR(const Vector2& position, const Vector2& size, float radius, float thickness, float sharpness, const Vector4& color);

		void AddTextCharacter(const Vector2& position, const Vector2& size, float thickness, float sharpness, const Vector4& color, Texture2D* texture, const Vector2& texturePosition, const Vector2& textureSize);

		void AddTextLine(const Vector2& position, const std::string& content, TextFont* font, float size, float thickness, float sharpness, const Vector4& color);

	private:
		void SetPipeline(PipelineType type);

		uchar stencilDepth = 0;

		usize drawCount = 0;

		PipelineType pipelineType;

		MeshPipeline meshPipeline;
		CirclePipeline circlePipeline;
		RRPipeline rrPipeline;
		TextPipeline textPipeline;

		Texture2D* defaultTexture = nullptr;
	};
}
