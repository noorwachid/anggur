#pragma once

#include "Anggur/Utility/Log.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector3.h"
#include "Anggur/Math/Vector4.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Graphics/Texture2D.h"
#include <vector>
#include <memory>

namespace Anggur 
{
	struct MeshVertex 
	{
		Vector3 position;
		Vector3 normal;
		Vector4 color;
		Vector2 textureCoord;
		float textureSlot;

		MeshVertex() = default;
		MeshVertex(const Vector3& newPosition, const Vector3& newNormal, const Vector4& newColor, const Vector2& newTextureCoord, float newTextureSlot = 0.0f): 
			position(newPosition), normal(newNormal), color(newColor), textureCoord(newTextureCoord), textureSlot(newTextureSlot) 
		{
		}

		std::string ToString()
		{
			std::string temp;

			temp += "position: " + position.ToString() + "\n";
			temp += "normal: " + normal.ToString() + "\n";
			temp += "color: " + color.ToString() + "\n";
			temp += "textureCoord: " + textureCoord.ToString() + "\n";
			temp += "textureSlot: " + std::to_string(textureSlot) + "\n"; 

			return temp;
		}
	};

	class MeshRenderer 
	{
	public:
		static void Initialize();

		static void SetBatchChunk(size_t vertex, size_t indexMultiplier = 2);
		static void Clear(const Vector4& color = Vector4::black);
		static void SetViewport(const Vector2& size);
		static void SetViewport(const Vector2& position, const Vector2& size);
		static void SetViewProjection(const Matrix4& newViewProjection);

		static void Begin();
		static void Begin(const Matrix4& viewProjection);
		static void End();

		static bool IsCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize);
		static void Flush();
		static void FlushData();

		static void Render(const std::vector<MeshVertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture);

		// 3D Primitives
		static void RenderCube(const Matrix4& model, const std::shared_ptr<Texture2D>& texture, const Vector4& color = Vector4::white);
		
		// 3D Primitive Tests
		static void RenderTestCube(const Matrix4& model);

		// 2D Primitives
		static void RenderTriangle(const Matrix4& model, const Vector3& point0, const Vector3& point1, const Vector3& point2, const Vector4& color = Vector4::white);
		static void RenderQuad(const Matrix4& model, const Vector3& point0, const Vector3& point1, const Vector3& point2, const Vector3& point3, const Vector4& color = Vector4::white);
		static void RenderRectangle(const Matrix4& model, const Vector3& point0, const Vector3& point1, const Vector4& color = Vector4::white);
		static void RenderTexturedRectangle(const Matrix4& model, const Vector3& point0, const Vector3& point1, const std::shared_ptr<Texture2D>& texture, const Vector2& texturePoint0 = Vector2::zero, const Vector2& texturePoint1 = Vector2::one, const Vector4& color = Vector4::white);

		static void RenderArc(const Matrix4& model, float radius = 1.0f, float beginAngle = 0.0f, float sweepAngle = Math::pi, int segment = 16, const Vector4& color = Vector4::white);
		static void RenderCircle(const Matrix4& model, float radius = 1.0f, int segment = 32, const Vector4& color = Vector4::white);

		// 2D Lines
		static void RenderLineTerminator(const Matrix4& model, const Vector2& point0, const Vector2& point1, float thickness = 0.5, const Vector4& color = Vector4::white);
		static void RenderLineAnchor(const Matrix4& model, const Vector2& point0, const Vector2& point1, const Vector2& p2, float thickness = 0.5, const Vector4& color = Vector4::white);
		static void RenderLine(const Matrix4& model, const Vector2& point0, const Vector2& point1, float thickness = 0.5, const Vector4& color = Vector4::white);
		static void RenderPolyLine(const Matrix4& model, const std::vector<Vector2>& points, float thickness = 0.5, const Vector4& color = Vector4::white);
		static void RenderPolyLineConnected(const Matrix4& model, const std::vector<Vector2>& points, float thickness = 0.5, const Vector4& color = Vector4::white);

		// 2D Texts
		enum class TextAlign
		{
			Left,
			Center,
			Right,
			Justify,
		};

		static void RenderText(const Matrix4& model, const Vector2& point0, const Vector2& point1, const std::string& textBuffer, TextAlign textVerticalAlign, TextAlign textHorizontalAlign, const Vector4& color = Vector4::white);

	private:
		MeshRenderer();

		static void InitializeVertexPool();
		static void InitializeTexturePool();
		static void InitializeShader();
	};
}