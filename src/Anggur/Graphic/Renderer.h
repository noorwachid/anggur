#pragma once
#include <Anggur/Math/Vector2.h>
#include <Anggur/Math/Vector4.h>
#include <Anggur/Math/Matrix3.h>
#include <Anggur/Graphic/Font.h>
#include <Anggur/Graphic/Shader.h>
#include <Anggur/Graphic/Texture.h>
#include <Anggur/Graphic/VertexArray.h>
#include <Anggur/Graphic/RendererStruct.h>
#include <vector>

namespace Anggur {
	struct Vertex {
		Vector2 position;
		Vector4 color;
		Vector2 texCoord;
		float texIndex;

		static const size_t length = 9;

		Vertex(const Vector2& position = Vector2::zero, const Vector4& color = Vector4::white, const Vector2& texCoord = Vector2::zero, float texIndex = -1):
			position(position), color(color), texCoord(texCoord), texIndex(texIndex) {
		}
	};

	class Renderer {
	public:
		static Shader& getBatchShader();

		static void createBatchShader();
		static void initialize();
		static void terminate();

		static void setDrawMode(DrawMode mode);
		static void setMaxQuad(size_t max = 2048);
		static void setCircleSegment(size_t segment);

		static void setViewport(Vector2 size);
		static void setViewProjection(const Matrix3& vp);

		static void clear(const Vector4& color = Vector4::black);

		static void checkCapacityLimit(size_t vertexOffset = 0, size_t indexOffset = 0, size_t textureOffset = 0);
		static void draw(const float* vertexData, size_t vertexLength, const uint32_t* indexData, size_t indexLength);
		static void draw(const Texture& texture);
		static void clearData();
		static void render();

		// NEW API

		// -- Primitives
		static void drawTriangle(const Matrix3& transform, const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector4& c = Vector4::white);
		static void drawQuad(const Matrix3& transform, const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3, const Vector4& c = Vector4::white);
		static void drawRectangle(const Matrix3& transform, const Vector2& p0, const Vector2& p1, const Vector4& color = Vector4::white);
		static void drawPolygon(const Matrix3& transform, const Vector2& p0, float r = 0.5, size_t segments = 6, const Vector4& c = Vector4::white);
		static void drawCircle(const Matrix3& transform, const Vector2& p0, float r = 0.5, const Vector4& c = Vector4::white);

		// -- Vertex Primitives
		static void drawQuad(const Matrix3& transform, const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3);

		// -- Lines
		static void drawLineTerminator(const Matrix3& transform, const Vector2& p0, const Vector2& p1, float w = 0.5, const Vector4& c = Vector4::white);
		static void drawLineAnchor(const Matrix3& transform, const Vector2& p0, const Vector2& p1, const Vector2& p2, float w = 0.5, const Vector4& c = Vector4::white);
		static void drawLine(const Matrix3& transform, const Vector2& p0, const Vector2& p1, float w = 0.5, const Vector4& c = Vector4::white);
		static void drawPolyLine(const Matrix3& transform, const std::vector<Vector2>& ps, float w = 0.5, const Vector4& c = Vector4::white);
		static void drawPolyLineConnected(const Matrix3& transform, const std::vector<Vector2>& ps, float w = 0.5, const Vector4& c = Vector4::white);

		// -- Curves
		static void drawQuadraticBezier(const Matrix3& transform, const Vector2& p0, const Vector2& p1, const Vector2& p2, float w = 0.5, const Vector4& c = Vector4::white);
		static void drawQuadraticBezierAlt(const Matrix3& transform, const Vector2& p0, const Vector2& p1, const Vector2& p2, float w = 0.5, const Vector4& c = Vector4::white);
		static void drawQubicBezier(const Matrix3& transform, const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3, float w = 0.5, const Vector4& c = Vector4::white);

		// -- Natural
		// TODO DrawArc
		static void drawConvex(const Matrix3& transform, const std::vector<Vector2>& ps, const Vector4& c = Vector4::white);

		// -- Text
		static void drawText(const Matrix3& transform, const Vector2& p0, const Vector2& p1, const std::string& textBuffer, const TextOption& textOption, Font& textFont, const Vector4& color = Vector4::white);
		static void drawText(const Matrix3& transform, const Vector2& p0, const std::vector<CodepointContainer>& ccs, Font& textFont, const Vector4& color);

		// OLD API
		// -- Textures
		static void drawTexture(const Vector2& p0, const Texture& t, const Vector4& c = Vector4::white);
		static void drawTexturedQuad(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3,
			const Vector2& t0, const Vector2& t1, const Vector2& t2, const Vector2& t3,
			const Texture& t, const Vector4& c = Vector4::white);
		static void drawTexturedRectangle(const Vector2& p0, float w, float h, const Texture& t, const Vector4& c = Vector4::white);

	private:
		static Shader _batchShader;

		static float* _vertexData;
		static unsigned int* _indexData;

		static size_t _vertexCounter;
		static size_t _indexCounter;

		static size_t _maxQuad;
		static size_t _circleSegment;

		static size_t _maxVertices;
		static size_t _maxIndices;

		static size_t _maxTextureUnits;
		static size_t _textureCounter;
		static float _textureIndex;
		static int* _textureIndices;
		static Texture* _textureData;
		static Matrix3 _viewProjectionMatrix;

		static VertexArray _vertexArray;
		static VertexBuffer _vertexBuffer;
		static IndexBuffer _indexBuffer;
	};
}
