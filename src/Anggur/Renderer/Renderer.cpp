#include "Renderer.h"
#include <Anggur/Helper/Log.h>
#include <glad/glad.h>

#define ANGGUR_DEBUG_TEXT_RECTS 1

namespace Anggur
{

	Shader Renderer::_batchShader;

	float* Renderer::_vertexData;

	uint* Renderer::_indexData;

	size_t Renderer::_vertexCounter;

	size_t Renderer::_indexCounter;

	size_t Renderer::_maxQuad = 2048;

	size_t Renderer::_circleSegment = 32;

	size_t Renderer::_maxVertices;

	size_t Renderer::_maxIndices;

	size_t Renderer::_maxTextureUnits;

	size_t Renderer::_textureCounter;

	float Renderer::_textureIndex;

	int* Renderer::_textureIndices;

	Texture* Renderer::_textureData;

	Matrix Renderer::_viewProjectionMatrix;

	VertexArray Renderer::_vertexArray;

	VertexBuffer Renderer::_vertexBuffer;

	IndexBuffer Renderer::_indexBuffer;

	Shader& Renderer::GetBatchShader()
	{
		return _batchShader;
	}

	void Renderer::CreateBatchShader()
	{
		_batchShader.SetVertexSource(R"(
        #version 330 core
        layout (location = 0) in vec2  aPosition;
        layout (location = 1) in vec4  aColor;
        layout (location = 2) in vec2  aTexCoord;
        layout (location = 3) in float aTexIndex;

        out vec4  vColor;
        out vec2  vTexCoord;
        out float vTexIndex;

        uniform mat3 uViewProjection;

        void main()
        {
        gl_Position       = vec4(uViewProjection * vec3(aPosition.x, aPosition.y, 1.0f), 1.0f);
        vColor      	  = aColor;
        vTexCoord   	  = aTexCoord;
        vTexIndex   	  = aTexIndex;
        }
    )");

		string fragmentSource = R"(
        #version 330 core

        layout (location = 0) out vec4 aColor;

        in vec4  vColor;
        in vec2  vTexCoord;
        in float vTexIndex;

        uniform sampler2D uTex[16];

        void main()
        {
            int index = int(vTexIndex);

            switch (index)
            {
            // MULTI CHANNELS TEXTURE
            case -1:
                aColor = vColor;
                break;
    )";

		int maxUnit;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnit);
		_maxTextureUnits = maxUnit;

		_textureData = new Texture[_maxTextureUnits];
		_textureIndices = new int[_maxTextureUnits];

		for (size_t i = 0; i < _maxTextureUnits; ++i)
		{
			_textureIndices[i] = i;

			fragmentSource += "case " + std::to_string(i) + ":\n";
			fragmentSource += "aColor = texture(uTex[" + std::to_string(i) + "], vTexCoord) * vColor;\n";
			fragmentSource += "break;\n";

			fragmentSource += "case " + std::to_string(i + _maxTextureUnits) + ":\n";
			fragmentSource +=
				"aColor = vec4(1.f, 1.f, 1.f, texture(uTex[" + std::to_string(i) + "], vTexCoord)) * vColor;\n";
			fragmentSource += "break;\n";
		}

		fragmentSource += R"(
            }
        }
    )";

		_batchShader.SetFragmentSource(fragmentSource);
		_batchShader.Compile();
	}

	void Renderer::Initialize()
	{
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		CreateBatchShader();

		_maxVertices = _maxQuad * 4;
		_maxIndices = _maxQuad * 6;

		_vertexArray.Create();
		_vertexArray.Bind();
		_vertexData = new float[_maxVertices * Vertex::length];
		_indexData = new uint[_maxIndices];

		_vertexBuffer.Create();
		_vertexBuffer.Bind();
		_vertexBuffer.SetCapacity(sizeof(float) * _maxVertices * Vertex::length);

		_vertexArray.SetAttributePtr(0, 2, sizeof(Vertex), (void*)offsetof(Vertex, position));
		_vertexArray.SetAttributePtr(1, 4, sizeof(Vertex), (void*)offsetof(Vertex, color));
		_vertexArray.SetAttributePtr(2, 2, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
		_vertexArray.SetAttributePtr(3, 1, sizeof(Vertex), (void*)offsetof(Vertex, texIndex));

		_indexBuffer.Create();
		_indexBuffer.Bind();
		_indexBuffer.SetCapacity(sizeof(uint) * _maxIndices);

		_viewProjectionMatrix = Matrix::identity;

		ClearData();
	}

	void Renderer::Terminate()
	{
		delete[] _vertexData;
		delete[] _indexData;
		delete[] _textureData;
		delete[] _textureIndices;

		_vertexData = nullptr;
		_indexData = nullptr;
		_textureData = nullptr;
		_textureIndices = nullptr;

		_indexBuffer.Destroy();
		_vertexBuffer.Destroy();
		_vertexArray.Destroy();
		_batchShader.Destroy();
	}

	void Renderer::SetDrawMode(DrawMode mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, static_cast<int>(mode));
	}

	void Renderer::SetMaxQuad(size_t max)
	{
		if (max < _circleSegment)
			max = _circleSegment;
		_maxQuad = max;
	}

	void Renderer::SetCircleSegment(size_t segment)
	{
		if (segment < 3)
			segment = 3;
		if (_maxQuad < segment)
			_maxQuad = segment;
		_circleSegment = segment;
	}

	void Renderer::SetViewport(Vector size)
	{
		glViewport(0, 0, size.x, size.y);
	}

	void Renderer::SetViewProjection(const Matrix& vp)
	{
		_viewProjectionMatrix = vp;
	}

	void Renderer::ClearBackground(const Color& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::CheckLimit(size_t vertexOffset, size_t indexOffset, size_t textureOffset)
	{
		if (_vertexCounter + vertexOffset > _maxVertices)
			return Render();
		if (_indexCounter + indexOffset > _maxIndices)
			return Render();
		if (_textureCounter + textureOffset > _maxTextureUnits)
			return Render();
	}

	void Renderer::AddData(const float* vetices, size_t vertexLength, const uint* indices, size_t indexLength)
	{
		size_t vertexOffset = _vertexCounter * Vertex::length;
		size_t vertexSize = vertexLength * Vertex::length;

		for (size_t i = 0; i < vertexSize; ++i)
			_vertexData[vertexOffset + i] = vetices[i];

		for (size_t i = 0; i < indexLength; ++i)
			_indexData[_indexCounter + i] = _vertexCounter + indices[i];

		_vertexCounter += vertexLength;
		_indexCounter += indexLength;
	}

	void Renderer::AddTextureData(const Texture& texture)
	{
		for (size_t i = 0; i < _textureCounter; ++i)
		{
			if (_textureData[i].GetID() == texture.GetID())
			{
				_textureIndex = i;
				return;
			}
		}

		_textureData[_textureCounter] = texture;
		_textureIndex = _textureCounter;
		_textureCounter++;
	}

	void Renderer::ClearData()
	{
		_vertexCounter = 0;
		_indexCounter = 0;
		_textureCounter = 0;
	}

	void Renderer::Render()
	{
		if (_vertexCounter == 0)
			return;

		_batchShader.Bind();
		_batchShader.SetMatrix("uViewProjection", _viewProjectionMatrix);
		_batchShader.SetInt("uTex", _maxTextureUnits, _textureIndices);

		for (size_t i = 0; i < _textureCounter; ++i)
			_textureData[i].Bind(i);

		_vertexBuffer.Bind();
		_vertexBuffer.SetData(sizeof(float) * _maxVertices * Vertex::length, _vertexData);

		_indexBuffer.Bind();
		_indexBuffer.SetData(sizeof(uint) * _maxIndices, _indexData);

		_vertexArray.Bind();
		glDrawElements(GL_TRIANGLES, _indexCounter, GL_UNSIGNED_INT, nullptr);

		ClearData();
	}


// Primitive geometries

	void
	Renderer::AddTriangle(const Matrix& transform, const Vector& p0, const Vector& p1, const Vector& p2, const Color& c)
	{
		CheckLimit(3, 3);

		Vector l0 = p0 * transform;
		Vector l1 = p1 * transform;
		Vector l2 = p2 * transform;

		float vertices[] = {
			l0.x, l0.y, c.r, c.g, c.b, c.a, 0.f, 0.f, -1,
			l1.x, l1.y, c.r, c.g, c.b, c.a, 0.f, 0.f, -1,
			l2.x, l2.y, c.r, c.g, c.b, c.a, 0.f, 0.f, -1,
		};

		uint indices[] = { 0, 1, 2 };

		AddData(vertices, 3, indices, 3);
	}

	void Renderer::AddQuad(const Matrix& transform,
		const Vector& p0,
		const Vector& p1,
		const Vector& p2,
		const Vector& p3,
		const Color& c)
	{
		CheckLimit(4, 6);

		Vector l0 = p0 * transform;
		Vector l1 = p1 * transform;
		Vector l2 = p2 * transform;
		Vector l3 = p3 * transform;

		float vertices[] = {
			l0.x,
			l0.y,
			c.r,
			c.g,
			c.b,
			c.a,
			0.f,
			0.f,
			-1,
			l1.x,
			l1.y,
			c.r,
			c.g,
			c.b,
			c.a,
			0.f,
			0.f,
			-1,
			l2.x,
			l2.y,
			c.r,
			c.g,
			c.b,
			c.a,
			0.f,
			0.f,
			-1,
			l3.x,
			l3.y,
			c.r,
			c.g,
			c.b,
			c.a,
			0.f,
			0.f,
			-1,
		};

		uint indices[] = {
			0, 1, 2,
			2, 3, 0 };

		AddData(vertices, 4, indices, 6);
	}

	void
	Renderer::AddQuad(const Matrix& transform, const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3)
	{
		CheckLimit(4, 6);

		Vector l0 = v0.position * transform;
		Vector l1 = v1.position * transform;
		Vector l2 = v2.position * transform;
		Vector l3 = v3.position * transform;

		float vertices[] = {
			l0.x,
			l0.y,
			v0.color.r,
			v0.color.g,
			v0.color.b,
			v0.color.a,
			v0.texCoord.x,
			v0.texCoord.y,
			v0.texIndex,

			l1.x,
			l1.y,
			v1.color.r,
			v1.color.g,
			v1.color.b,
			v1.color.a,
			v1.texCoord.x,
			v1.texCoord.y,
			v1.texIndex,

			l2.x,
			l2.y,
			v2.color.r,
			v2.color.g,
			v2.color.b,
			v2.color.a,
			v2.texCoord.x,
			v2.texCoord.y,
			v2.texIndex,

			l3.x,
			l3.y,
			v3.color.r,
			v3.color.g,
			v3.color.b,
			v3.color.a,
			v3.texCoord.x,
			v3.texCoord.y,
			v3.texIndex,
		};

		uint indices[] = {
			0, 1, 2,
			2, 3, 0 };

		AddData(vertices, 4, indices, 6);
	}

	void Renderer::AddRect(const Matrix& transform, const Vector& p0, const Vector& p1, const Color& color)
	{
		AddQuad(transform,
			p0 * transform,
			Vector(p0.x, p1.y) * transform,
			p1 * transform,
			Vector(p1.x, p0.y) * transform,
			color);
	}

	void Renderer::AddPolygon(const Matrix& transform, const Vector& p0, float r, size_t segments, const Color& c)
	{
		if (segments < 3)
			segments = 3;

		size_t triangles = segments - 2;

		CheckLimit(segments, triangles * 3);

		float theta = Math::twoPi / segments;
		float tangetialFactor = Math::Tan(theta);
		float radialFactor = Math::Cos(theta);

		float x = r;
		float y = 0;

		float vertices[segments * Vertex::length];
		uint indices[triangles * 3];

		for (size_t i = 0, offset = 0; i < segments; i++)
		{
			Vector point = Vector(p0.x + x, p0.y + y) * transform;
			vertices[offset + 0] = x + point.x;
			vertices[offset + 1] = y + point.y;

			vertices[offset + 2] = c.r;
			vertices[offset + 3] = c.g;
			vertices[offset + 4] = c.b;
			vertices[offset + 5] = c.a;

			vertices[offset + 6] = 0;
			vertices[offset + 7] = 0;

			vertices[offset + 8] = -1;

			float tx = -y;
			float ty = x;

			x += tx * tangetialFactor;
			y += ty * tangetialFactor;

			x *= radialFactor;
			y *= radialFactor;

			offset += Vertex::length;
		}

		for (size_t i = 0, offset = 0; i < triangles; ++i)
		{
			indices[offset + 0] = 0;
			indices[offset + 1] = i + 1;
			indices[offset + 2] = i + 2;
			offset += 3;
		}

		AddData(vertices, segments, indices, triangles * 3);
	}

	void Renderer::AddCircle(const Matrix& transform, const Vector& p0, float r, const Color& c)
	{
		AddPolygon(transform, p0, r, _circleSegment, c);
	}

// Complex geometries

	void
	Renderer::AddTerminator(const Matrix& transform, const Vector& p0, const Vector& p1, float weight, const Color& c)
	{
		Vector p3 = (p0 - p1).Normalize() * weight;
		Vector t0 = (p1 - p0).GetPerpen().Normalize() * weight;

		AddQuad(transform, p1 + t0, p3 + p0 + t0, p3 + p0 - t0, p1 - t0, c);
	}

	void Renderer::AddAnchor(const Matrix& transform,
		const Vector& p0,
		const Vector& p1,
		const Vector& p2,
		float w0,
		const Color& c)
	{
		Vector i0 = Vector::zero * transform;
		Vector l0 = p0 * transform;
		Vector l1 = p1 * transform;
		Vector l2 = p2 * transform;
		Vector t0 = (l1 - l0).GetPerpen();
		Vector t2 = (l2 - l1).GetPerpen();

		if (0 < ((l1.x - l0.x) * (l2.y - l0.y) - (l2.x - l0.x) * (l1.y - l0.y)))
		{
			t0 = -t0;
			t2 = -t2;
		}

		t0.SetLength(w0);
		t2.SetLength(w0);

		Vector u0 = (l0 + t0);
		Vector u1 = (l2 + t2);
		Vector n0 = (l0 - t0);
		Vector n1 = (l2 - t2);
		Vector c0 = (l1 + t0);
		Vector c1 = (l1 + t2);
		Vector d0 = (l1 - t0);
		Vector d1 = (l1 - t2);
		Vector e0 = ((l1 - l0).SetLength(w0 * 2) + c0);
		Vector e1 = ((l1 - l2).SetLength(w0 * 2) + c1);

		auto areLinesIntersected = [](
			const Vector& p0,
			const Vector& p1,
			const Vector& p2,
			const Vector& p3,
			Vector& p4) -> bool
		{
			float denom = (p3.y - p2.y) * (p1.x - p0.x) - (p3.x - p2.x) * (p1.y - p0.y);
			float numea = (p3.x - p2.x) * (p0.y - p2.y) - (p3.y - p2.y) * (p0.x - p2.x);
			float numeb = (p1.x - p0.x) * (p0.y - p2.y) - (p1.y - p0.y) * (p0.x - p2.x);

			float denomAbs = Math::Abs(denom);
			float numeaAbs = Math::Abs(numea);
			float numebAbs = Math::Abs(numeb);

			if (numeaAbs < Math::epsilon && numebAbs < Math::epsilon && denomAbs < Math::epsilon)
			{
				p4 = Vector::Lerp(p0, p1, 0.5);
				return true;
			}

			if (denomAbs < Math::epsilon)
				return false;

			float mua = numea / denom;
			float mub = numeb / denom;

			if (mua < 0 || mua > 1 || mub < 0 || mub > 1)
			{
				return false;
			}

			float muax = numea / denom;
			p4 = (p1 - p0) * muax;
			p4 += p0;
			return true;
		};

		bool intersected = areLinesIntersected(c0, e0, c1, e1, i0);

#ifdef ANGGUR_DEBUG_ANCHOR_POINTS
		AddCircle(Matrix(), e0, 0.01, Color::red);
		AddCircle(Matrix(), e1, 0.01, Color::red);
		AddCircle(Matrix(), u0, 0.01, Color::blue);
		AddCircle(Matrix(), u1, 0.01, Color::blue);
		AddCircle(Matrix(), n0, 0.01, Color::green);
		AddCircle(Matrix(), n1, 0.01, Color::green);
		AddCircle(Matrix(), c0, 0.01, Color::yellow);
		AddCircle(Matrix(), c1, 0.01, Color::yellow);
		AddCircle(Matrix(), d0, 0.01, Color::pink);
		AddCircle(Matrix(), d1, 0.01, Color::pink);
		AddCircle(Matrix(), i0, 0.01, Color::cyan);
		AddCircle(Matrix(), l1, 0.01, Color::purple);
		return;
#endif

		float vertices[] = {
			e0.x, e0.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 0
			e1.x, e1.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 1
			u0.x, u0.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 2
			u1.x, u1.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 3
			n0.x, n0.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 4
			n1.x, n1.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 5
			c0.x, c0.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 6
			c1.x, c1.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 7
			d0.x, d0.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 8
			d1.x, d1.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 9
			i0.x, i0.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 10
			l1.x, l1.y, c.r, c.g, c.b, c.a, 0, 0, -1, // 11
		};

		uint indexLength = 21;
		uint indices[] = {
			2,
			6,
			8,
			8,
			4,
			2,
			7,
			3,
			5,
			5,
			9,
			7,
			6,
			7,
			11, // mid
			6,
			0,
			1,
			1,
			7,
			6,
		};

		if (intersected)
		{
			indexLength = 18;
			indices[15] = 6;
			indices[16] = 10;
			indices[17] = 7;
		}

		AddData(vertices, 12, indices, indexLength);
	}

	void Renderer::AddLine(const Matrix& transform, const Vector& p0, const Vector& p1, float weight, const Color& c)
	{
		Vector m0 = Vector::Lerp(p0, p1, 0.5);

		AddTerminator(transform, p0, m0, weight, c);
		AddTerminator(transform, p1, m0, weight, c);
	}

	void Renderer::AddPolyline(const Matrix& transform, const std::vector<Vector>& ps, float w, const Color& c)
	{
		if (ps.size() > 1)
		{
			std::vector<Vector> ms;

			for (size_t i = 0; i < ps.size() - 1; ++i)
				ms.push_back(Vector::Lerp(ps[i], ps[i + 1], 0.5));

			for (size_t i = 1; i < ms.size(); ++i)
				AddAnchor(transform, ms[i - 1], ps[i], ms[i], w, c);

			AddTerminator(transform, ps.front(), ms.front(), w, c);
			AddTerminator(transform, ps.back(), ms.back(), w, c);
		}
	}

	void Renderer::AddPolyring(const Matrix& transform, const std::vector<Vector>& ps, float w, const Color& c)
	{
		if (ps.size() > 1)
		{
			std::vector<Vector> ms;

			for (size_t i = 0; i < ps.size() - 1; ++i)
				ms.push_back(Vector::Lerp(ps[i], ps[i + 1], 0.5));

			for (size_t i = 1; i < ms.size(); ++i)
				AddAnchor(transform, ms[i - 1], ps[i], ms[i], w, c);

			Vector m = Vector::Lerp(ps.front(), ps.back(), 0.5);

			AddAnchor(transform, m, ps.front(), ms.front(), w, c);
			AddAnchor(transform, ms.back(), ps.back(), m, w, c);
		}
	}

	void Renderer::AddQuadraticBz(const Matrix& transform,
		const Vector& p0,
		const Vector& p1,
		const Vector& p2,
		float w,
		const Color& c)
	{
		auto GetLerped = [](const Vector& p0, const Vector& p1, const Vector& p2, float t)
		{
			Vector pt;

			float t2 = t * 2;
			float tq = t * t;
			float ti = 1.f - t;
			float tiq = ti * ti;

			pt.x = tiq * p0.x +
				ti * t2 * p1.x +
				tq * p2.x;
			pt.y = tiq * p0.y +
				ti * t2 * p1.y +
				tq * p2.y;

			return pt;
		};

		std::vector<Vector> points;

		for (int i = 0; i <= 10; ++i)
			points.push_back(GetLerped(p0, p1, p2, i / 10.f));

		AddPolyline(transform, points, w, c);
	}

	void Renderer::AddQuadraticBzi(const Matrix& transform,
		const Vector& p0,
		const Vector& p1,
		const Vector& p2,
		float w,
		const Color& c)
	{
		Vector px = p1 * 2 - (p0 + p2) / 2;
		AddQuadraticBz(transform, p0, px, p2, w, c);
	}

	void Renderer::AddQubicBz(const Matrix& transform,
		const Vector& p0,
		const Vector& p1,
		const Vector& p2,
		const Vector& p3,
		float w,
		const Color& c)
	{
		auto GetLerped = [](const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, float t)
		{
			Vector pt;

			float t3 = t * 3;
			float tc = t * t * t;
			float ti = 1.f - t;
			float tiq = ti * ti;
			float tic = ti * ti * ti;

			pt.x = tic * p0.x +
				tiq * t3 * p1.x +
				ti * t3 * t * p2.x +
				tc * p3.x;
			pt.y = tic * p0.y +
				tiq * t3 * p1.y +
				ti * t3 * t * p2.y +
				tc * p3.y;

			return pt;
		};

		std::vector<Vector> points;

		for (int i = 0; i <= 10; ++i)
			points.push_back(GetLerped(p0, p1, p2, p3, i / 10.f));

		AddPolyline(transform, points, w, c);
	}

// Natural geometries

	void Renderer::AddConvex(const Matrix& transform, const std::vector<Vector>& ps, const Color& c)
	{
		size_t triangles = ps.size() - 2;
		uint indices[triangles * 3];
		float vertices[ps.size() * Vertex::length];

		for (size_t i = 0, offset = 0; i < ps.size(); ++i)
		{
			vertices[offset + 0] = ps[i].x;
			vertices[offset + 1] = ps[i].y;

			vertices[offset + 2] = c.r;
			vertices[offset + 3] = c.g;
			vertices[offset + 4] = c.b;
			vertices[offset + 5] = c.a;

			vertices[offset + 6] = 0;
			vertices[offset + 7] = 0;

			vertices[offset + 8] = -1;

			offset += Vertex::length;
		}

		for (size_t i = 0, offset = 0; i < triangles; ++i)
		{
			indices[offset + 0] = 0;
			indices[offset + 1] = i + 1;
			indices[offset + 2] = i + 2;

			offset += 3;
		}

		AddData(vertices, ps.size(), indices, triangles * 3);
	}


// Text

	void Renderer::AddText(const Matrix& transform,
		const Vector& p0,
		const Vector& p1,
		const std::string& textBuffer,
		const TextOption& textOption,
		Font& textFont,
		const Color& color)
	{
		Vector offset = p0;
		Vector containerArea = p1 - p0;
		Vector occupiedArea;
		Vector lineArea;
		Vector wordOffset;
		Vector wordArea;

		occupiedArea.y = -textFont.GetLineHeight() * textOption.size * textOption.lineHeight;
		lineArea.y = occupiedArea.y;
		wordArea.y = occupiedArea.y - textOption.size * 0.3;

		AddRect(transform, p0, p1, Color(0, 0, 1, 0.4));

		bool isFit = true;

		float wordSpace = textOption.size * textOption.wordSpace;
		float letterSpace = textOption.size * textOption.letterSpace;

		Vector ellipsisArea;
		std::vector<CodepointContainer> ellipsisCcs;

		for (size_t i = 0; i < textOption.ellipsis.size(); ++i)
		{
			int codepoint = textOption.ellipsis[i];
			CodepointContainer cc;
			cc.glyph = textFont.glyphs[codepoint];
			cc.offset.x = ellipsisArea.x;
			cc.offset.y = textOption.size * cc.glyph.ascent;
			cc.area.x = textOption.size * cc.glyph.size.x;
			cc.area.y = textOption.size * cc.glyph.size.y;
			ellipsisCcs.push_back(cc);

			ellipsisArea.x += cc.area.x + letterSpace;
		}
		ellipsisArea.x -= letterSpace;

		std::vector<CodepointContainer> ccs;

		auto CreateNewWord = [&]()
		{
			wordOffset.x += wordSpace;

			lineArea.x += wordArea.x;
			lineArea.x += wordSpace;
			wordArea.x -= letterSpace;

#ifdef ANGGUR_DEBUG_TEXT_RECTS
			AddRect(transform, offset, offset + wordArea, Color(1, 1, 1, 0.33));
#endif
			AddTextChunk(transform, offset, ccs, textFont, color);

			offset.x += wordOffset.x;
			wordOffset.Set(0, 0);
			wordArea.x = 0;
			ccs.clear();
		};

		auto CreateNewLine = [&]()
		{
			offset.x = p0.x;
			lineArea.x -= wordSpace;
			lineArea.x -= letterSpace;

			occupiedArea.x = Math::Max(occupiedArea.x, lineArea.x);
			occupiedArea.y += lineArea.y;
#ifdef ANGGUR_DEBUG_TEXT_RECTS
			AddRect(transform, offset, offset + lineArea, Color(1, 1, 1, 0.33));
#endif

			lineArea.x = 0;
			offset.y += lineArea.y;
		};

		for (size_t i = 0; i < textBuffer.size(); ++i)
		{
			int codepoint = textBuffer[i];
			CodepointContainer cc;
			cc.glyph = textFont.glyphs[codepoint];
			cc.offset.x = wordOffset.x;
			cc.offset.y = textOption.size * cc.glyph.ascent;
			cc.area.x = textOption.size * cc.glyph.size.x;
			cc.area.y = textOption.size * cc.glyph.size.y;
			ccs.push_back(cc);

			if (codepoint == '\n')
			{
				CreateNewWord();
				CreateNewLine();
			}
			else if (codepoint == ' ')
			{
				if (lineArea.x + wordArea.x > containerArea.x)
				{

					if (occupiedArea.y + lineArea.y > containerArea.y)
					{
						occupiedArea.x = Math::Max(occupiedArea.x, lineArea.x);
#ifdef ANGGUR_DEBUG_TEXT_RECTS
						AddCircle(transform,
							p0 + Vector(occupiedArea.x, occupiedArea.y + lineArea.y),
							0.1,
							Color(1, 1, 0, 0.5));
#endif
						isFit = false;
						AddTextChunk(transform, offset, ellipsisCcs, textFont, color);
						break;
					}
					else
					{
						CreateNewLine();
						CreateNewWord();
					}
				}
				else
					CreateNewWord();
			}
			else
			{
				// TODO: Split the word if longer than the container area

				float x = letterSpace + cc.area.x + textFont.GetKerning(codepoint, textBuffer[i + 1]) * textOption.size;
				wordOffset.x += x;
				wordArea.x += x;
			}
		}

		if (isFit)
		{
			CreateNewLine();
			CreateNewWord();
		}

		offset.x = p0.x;
#ifdef ANGGUR_DEBUG_TEXT_RECTS
		AddRect(transform, offset, offset + lineArea, Color(1, 1, 1, 0.33));
		AddRect(transform, p0, p0 + occupiedArea, Color(1, 1, 1, 0.33));
		AddCircle(transform, p0 + containerArea, 0.1, Color::green);
#endif
	}

	void Renderer::AddTextChunk(const Matrix& transform,
		const Vector& p0,
		const std::vector<CodepointContainer>& ccs,
		Font& textFont,
		const Color& color)
	{
		for (const CodepointContainer& cc: ccs)
		{
#ifdef ANGGUR_DEBUG_TEXT_RECTS
			AddRect(transform, p0 + cc.offset, p0 + cc.offset + cc.area, Color(1, 1, 1, 0.33));
#endif
			AddQuadx(
				Vector(p0.x + cc.offset.x, p0.y + cc.offset.y) * transform,
				Vector(p0.x + cc.offset.x + cc.area.x, p0.y + cc.offset.y) * transform,
				Vector(p0.x + cc.offset.x + cc.area.x, p0.y + cc.offset.y + cc.area.y) * transform,
				Vector(p0.x + cc.offset.x, p0.y + cc.offset.y + cc.area.y) * transform,

				Vector(cc.glyph.x, cc.glyph.y),
				Vector(cc.glyph.x + cc.glyph.w, cc.glyph.y),
				Vector(cc.glyph.x + cc.glyph.w, cc.glyph.y + cc.glyph.h),
				Vector(cc.glyph.x, cc.glyph.y + cc.glyph.h),
				textFont.GetTexture(),
				color);
		}
	}

// Textured geometries

	void Renderer::AddQuadx(const Vector& p0,
		const Vector& p1,
		const Vector& p2,
		const Vector& p3,
		const Vector& t0,
		const Vector& t1,
		const Vector& t2,
		const Vector& t3,
		const Texture& t,
		const Color& c)
	{
		CheckLimit(4, 6, 1);

		AddTextureData(t);
		float ti = _textureIndex;
		if (t.GetChannels() == 1)
			ti += _maxTextureUnits;

		float vertices[] = {
			p0.x,
			p0.y,
			c.r,
			c.g,
			c.b,
			c.a,
			t0.x,
			t0.y,
			ti,
			p1.x,
			p1.y,
			c.r,
			c.g,
			c.b,
			c.a,
			t1.x,
			t1.y,
			ti,
			p2.x,
			p2.y,
			c.r,
			c.g,
			c.b,
			c.a,
			t2.x,
			t2.y,
			ti,
			p3.x,
			p3.y,
			c.r,
			c.g,
			c.b,
			c.a,
			t3.x,
			t3.y,
			ti,
		};

		uint indices[] = {
			0,
			1,
			2,
			2,
			3,
			0,
		};

		AddData(vertices, 4, indices, 6);
	}

	void Renderer::Addx(const Vector& p0, const Texture& t, const Color& c)
	{
		float w = t.GetWidth();
		float h = t.GetHeight();

		AddQuadx(
			p0,
			{ p0.x + w, p0.y },
			{ p0.x + w, p0.y + h },
			{ p0.x, p0.y + h },
			{ 0.f, 0.f },
			{ 1.f, 0.f },
			{ 1.f, 1.f },
			{ 0.f, 1.f },
			t,
			c);
	}

	void Renderer::AddRectx(const Vector& p0, float w, float h, const Texture& t, const Color& c)
	{
		AddQuadx(
			p0,
			{ p0.x + w, p0.y },
			{ p0.x + w, p0.y + h },
			{ p0.x, p0.y + h },
			{ 0, 0 },
			{ 1, 0 },
			{ 1, 1 },
			{ 0, 1 },
			t,
			c);
	}

	void Renderer::AddBoxx(const Vector& position, const Vector& radii, const Texture& t, const Color& c)
	{
		AddQuadx(
			{ position.x - radii.x, position.y - radii.y },
			{ position.x + radii.x, position.y - radii.y },
			{ position.x + radii.x, position.y + radii.y },
			{ position.x - radii.x, position.y + radii.y },
			{ 0, 0 },
			{ 1, 0 },
			{ 1, 1 },
			{ 0, 1 },
			t,
			c);
	}

} // namespace Anggur
