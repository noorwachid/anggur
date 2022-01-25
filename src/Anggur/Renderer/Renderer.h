#pragma once
#include "Color.h"
#include "Font.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "RendererStruct.h"
#include <Anggur/Math/Matrix.h>
#include <Anggur/Math/Vector.h>
#include <vector>

namespace Anggur
{
	struct Vertex
	{
		Vector position;
		Color color;
		Vector texCoord;
		float texIndex;

		static const size_t length = 9;

		Vertex(const Vector& position = Vector::zero,
			const Color& color = Color::white,
			const Vector& texCoord = Vector::zero,
			float texIndex = -1)
			:
			position(position), color(color), texCoord(texCoord), texIndex(texIndex)
		{
		}
	};

	class Renderer
	{
	public:
		static Shader& GetBatchShader();

		static void CreateBatchShader();
		static void Initialize();
		static void Terminate();

		static void SetDrawMode(DrawMode mode);
		static void SetMaxQuad(size_t max = 2048);
		static void SetCircleSegment(size_t segment);

		static void SetViewport(Vector size);
		static void SetViewProjection(const Matrix& vp);

		static void ClearBackground(const Color& color = Color::black);
		static void CheckLimit(size_t vertexOffset = 0, size_t indexOffset = 0, size_t textureOffset = 0);
		static void AddData(const float* vertexData, size_t vertexLength, const uint* indexData, size_t indexLength);
		static void AddTextureData(const Texture& texture);
		static void ClearData();
		static void Render();

		// NEW API

		// -- Primitives
		static void AddTriangle(const Matrix& transform,
			const Vector& p0 = Vector(0.0, -0.5),
			const Vector& p1 = Vector(0.5, 0.5),
			const Vector& p2 = Vector(-0.5, 0.5),
			const Color& c = Color::white);
		static void AddQuad(const Matrix& transform,
			const Vector& p0 = Vector(-0.5, -0.5),
			const Vector& p1 = Vector(0.5, -0.5),
			const Vector& p2 = Vector(0.5, 0.5),
			const Vector& p3 = Vector(-0.5, 0.5),
			const Color& c = Color::white);
		static void AddRect(const Matrix& transform,
			const Vector& p0 = Vector(-0.5, -0.5),
			const Vector& p1 = Vector(0.5, 0.5),
			const Color& color = Color::white);
		static void AddPolygon(const Matrix& transform,
			const Vector& p0,
			float r = 0.5,
			size_t segments = 6,
			const Color& c = Color::white);
		static void AddCircle(const Matrix& transform, const Vector& p0, float r = 0.5, const Color& c = Color::white);

		// -- Vertex Primitives
		static void
		AddQuad(const Matrix& transform, const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3);

		// -- Lines
		static void AddTerminator(const Matrix& transform,
			const Vector& p0,
			const Vector& p1,
			float w = 0.5,
			const Color& c = Color::white);
		static void AddAnchor(const Matrix& transform,
			const Vector& p0,
			const Vector& p1,
			const Vector& p2,
			float w = 0.5,
			const Color& c = Color::white);
		static void AddLine(const Matrix& transform,
			const Vector& p0,
			const Vector& p1,
			float w = 0.5,
			const Color& c = Color::white);
		static void AddPolyline(const Matrix& transform,
			const std::vector<Vector>& ps,
			float w = 0.5,
			const Color& c = Color::white);
		static void AddPolyring(const Matrix& transform,
			const std::vector<Vector>& ps,
			float w = 0.5,
			const Color& c = Color::white);

		// -- Curves
		static void AddQuadraticBz(const Matrix& transform,
			const Vector& p0,
			const Vector& p1,
			const Vector& p2,
			float w = 0.5,
			const Color& c = Color::white);
		static void AddQuadraticBzi(const Matrix& transform,
			const Vector& p0,
			const Vector& p1,
			const Vector& p2,
			float w = 0.5,
			const Color& c = Color::white);
		static void AddQubicBz(const Matrix& transform,
			const Vector& p0,
			const Vector& p1,
			const Vector& p2,
			const Vector& p3,
			float w = 0.5,
			const Color& c = Color::white);

		// -- Natural
		// TODO DrawArc
		static void AddConvex(const Matrix& transform, const std::vector<Vector>& ps, const Color& c = Color::white);

		// -- Text
		static void AddText(const Matrix& transform,
			const Vector& p0,
			const Vector& p1,
			const string& textBuffer,
			const TextOption& textOption,
			Font& textFont,
			const Color& color = Color::white);
		static void AddTextChunk(const Matrix& transform,
			const Vector& p0,
			const std::vector<CodepointContainer>& ccs,
			Font& textFont,
			const Color& color);

		// OLD API
		// -- Textures
		static void Addx(const Vector& p0, const Texture& t, const Color& c = Color::white);
		static void AddQuadx(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3,
			const Vector& t0, const Vector& t1, const Vector& t2, const Vector& t3,
			const Texture& t, const Color& c = Color::white);
		static void AddRectx(const Vector& p0, float w, float h, const Texture& t, const Color& c = Color::white);
		static void
		AddBoxx(const Vector& position, const Vector& radii, const Texture& t, const Color& c = Color::white);

	private:
		static Shader _batchShader;

		static float* _vertexData;
		static uint* _indexData;

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
		static Matrix _viewProjectionMatrix;

		static VertexArray _vertexArray;
		static VertexBuffer _vertexBuffer;
		static IndexBuffer _indexBuffer;
	};
}
