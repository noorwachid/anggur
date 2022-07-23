#pragma once

#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector4.h"
#include "Anggur/Math/Matrix3.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "Texture2D.h"
#include <vector>
#include <memory>

namespace Anggur {
	struct CanvasVertex {
		Vector2 position;
		Vector4 color;
		Vector2 uv;
		float slot;

		CanvasVertex() = default;
		CanvasVertex(const Vector2& newPosition, const Vector4& newColor, const Vector2& newUV, float newSlot = 0.0f): 
			position(newPosition), color(newColor), uv(newUV), slot(newSlot) {
		}

		std::string toString() {
			std::string temp;

			temp += "position: " + position.toString() + "\n";
			temp += "color: " + color.toString() + "\n";
			temp += "uv: " + uv.toString() + "\n";
			temp += "slot: " + std::to_string(slot) + "\n"; 

			return temp;
		}
	};

	class CanvasRenderer {
	public:
        CanvasRenderer();

        ~CanvasRenderer();

		void setBatchChunk(size_t vertex, size_t indexMultiplier = 2);
		void clear(const Vector4& color = Vector4::black);
		void setViewport(const Vector2& size);
		void setViewport(const Vector2& position, const Vector2& size);
		void setViewProjection(const Matrix3& newViewProjection);

		void begin();
		void begin(const Matrix3& viewProjection);
		void end();

		bool isCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize);
		void flush();
		void flushInternalBuffer();

		void draw(const std::vector<CanvasVertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture);

		// 2D Primitives
		void drawTriangle(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& point2, const Vector4& color = Vector4::white);
		void drawQuad(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& point2, const Vector2& point3, const Vector4& color = Vector4::white);
		void drawRectangle(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector4& color = Vector4::white);
		
        void drawTexturedRectangle(const Matrix3& model, const Vector2& point0, const Vector2& point1, const std::shared_ptr<Texture2D>& texture, const Vector2& texturePoint0 = Vector2::zero, const Vector2& texturePoint1 = Vector2::one, const Vector4& color = Vector4::white);

        // 2D Circles
		void drawArc(const Matrix3& model, float radius = 1.0f, float beginAngle = 0.0f, float sweepAngle = Math::pi, int segment = 16, const Vector4& color = Vector4::white);
		void drawCircle(const Matrix3& model, float radius = 1.0f, int segment = 32, const Vector4& color = Vector4::white);

		// 2D Lines
		void drawLineTerminator(const Matrix3& model, const Vector2& point0, const Vector2& point1, float thickness = 0.5, const Vector4& color = Vector4::white);
		void drawLineAnchor(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& p2, float thickness = 0.5, const Vector4& color = Vector4::white);
		void drawLine(const Matrix3& model, const Vector2& point0, const Vector2& point1, float thickness = 0.5, const Vector4& color = Vector4::white);
		void drawPolyLine(const Matrix3& model, const std::vector<Vector2>& points, float thickness = 0.5, const Vector4& color = Vector4::white);
		void drawClosedPolyLine(const Matrix3& model, const std::vector<Vector2>& points, float thickness = 0.5, const Vector4& color = Vector4::white);

		// 2D Texts
		enum class TextAlign {
			left,
			center,
			right,
			justify,
		};

		void drawText(const Matrix3& model, const Vector2& point0, const Vector2& point1, const std::string& textBuffer, TextAlign textVerticalAlign, TextAlign textHorizontalAlign, const Vector4& color = Vector4::white);

    private:
        std::shared_ptr<Shader> shader;
        std::shared_ptr<VertexArray> vertexArray;
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<IndexBuffer> indexBuffer;

        std::vector<CanvasVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<std::shared_ptr<Texture2D>> textures;
        std::vector<int> slots;

        std::shared_ptr<Texture2D> whiteTexture;

        size_t vertexOffset = 0;
        size_t indexOffset = 0;
        size_t textureOffset = 0;

        size_t renderCount = 0;
        size_t batchVertex = 128;
        size_t batchIndexMultiplier = 2;

        Matrix3 viewProjection;

	private:
		void initializeVertexPool();
		void initializeTexturePool();
		void initializeShader();

	};
}