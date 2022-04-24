// #include <Anggur/Utility/Log.h>
// #include <Anggur/Graphics/Function.h>
// #include <Anggur/Graphics/Renderer.h>

// #define ANGGUR_DEBUG_TEXT_RECTS 1

// namespace Anggur {

// 	Shader Renderer::_batchShader;

// 	float* Renderer::_vertexData;

// 	uint32_t* Renderer::_indexData;

// 	size_t Renderer::_vertexCounter;

// 	size_t Renderer::_indexCounter;

// 	size_t Renderer::_maxQuad = 2048;

// 	size_t Renderer::_circleSegment = 32;

// 	size_t Renderer::_maxVertices;

// 	size_t Renderer::_maxIndices;

// 	size_t Renderer::_maxTextureUnits;

// 	size_t Renderer::_textureCounter;

// 	float Renderer::_textureIndex;

// 	int* Renderer::_textureIndices;

// 	Texture* Renderer::_textureData;

// 	Matrix3 Renderer::_viewProjectionMatrix;

// 	VertexArray Renderer::_vertexArray;

// 	VertexBuffer Renderer::_vertexBuffer;

// 	IndexBuffer Renderer::_indexBuffer;

// 	Shader& Renderer::getBatchShader() {
// 		return _batchShader;
// 	}

// 	void Renderer::createBatchShader() {
// 		_batchShader.setVertexSource(R"(
//         #version 330 core
//         layout (location = 0) in vec2  aPosition;
//         layout (location = 1) in vec4  aColor;
//         layout (location = 2) in vec2  aTexCoord;
//         layout (location = 3) in float aTexIndex;

//         out vec4  vColor;
//         out vec2  vTexCoord;
//         out float vTexIndex;

//         uniform mat3 uViewProjection;

//         void main()
//         {
//         gl_Position       = vec4(uViewProjection * vec3(aPosition.x, aPosition.y, 1.0f), 1.0f);
//         vColor      	  = aColor;
//         vTexCoord   	  = aTexCoord;
//         vTexIndex   	  = aTexIndex;
//         }
//     )");

// 		std::string fragmentSource = R"(
//         #version 330 core

//         layout (location = 0) out vec4 aColor;

//         in vec4  vColor;
//         in vec2  vTexCoord;
//         in float vTexIndex;

//         uniform sampler2D uTex[16];

//         void main()
//         {
//             int index = int(vTexIndex);

//             switch (index)
//             {
//             // MULTI CHANNELS TEXTURE
//             case -1:
//                 aColor = vColor;
//                 break;
//     )";

// 		int maxUnit;
// 		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnit);
// 		_maxTextureUnits = maxUnit;

// 		_textureData = new Texture[_maxTextureUnits];
// 		_textureIndices = new int[_maxTextureUnits];

// 		for (size_t i = 0; i < _maxTextureUnits; ++i) {
// 			_textureIndices[i] = i;

// 			fragmentSource += "case " + std::to_string(i) + ":\n";
// 			fragmentSource += "aColor = texture(uTex[" + std::to_string(i) + "], vTexCoord) * vColor;\n";
// 			fragmentSource += "break;\n";

// 			fragmentSource += "case " + std::to_string(i + _maxTextureUnits) + ":\n";
// 			fragmentSource +=
// 				"aColor = vec4(1.f, 1.f, 1.f, texture(uTex[" + std::to_string(i) + "], vTexCoord)) * vColor;\n";
// 			fragmentSource += "break;\n";
// 		}

// 		fragmentSource += R"(
//             }
//         }
//     )";

// 		_batchShader.setFragmentSource(fragmentSource);
// 		_batchShader.compile();
// 	}

// 	void Renderer::initialize() {
// 		glEnable(GL_MULTISAMPLE);
// 		glEnable(GL_BLEND);
// 		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// 		createBatchShader();

// 		_maxVertices = _maxQuad * 4;
// 		_maxIndices = _maxQuad * 6;

// 		// _vertexArray.create();
// 		// _vertexArray.bind();
// 		// _vertexData = new float[_maxVertices * Vertex::length];
// 		// _indexData = new uint32_t[_maxIndices];

// 		// _vertexBuffer.create();
// 		// _vertexBuffer.bind();
// 		// _vertexBuffer.setCapacity(sizeof(float) * _maxVertices * Vertex::length);

// 		// _vertexArray.setAttribute(0, 2, sizeof(Vertex), (void*)offsetof(Vertex, position));
// 		// _vertexArray.setAttribute(1, 4, sizeof(Vertex), (void*)offsetof(Vertex, color));
// 		// _vertexArray.setAttribute(2, 2, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
// 		// _vertexArray.setAttribute(3, 1, sizeof(Vertex), (void*)offsetof(Vertex, texIndex));

// 		// _indexBuffer.create();
// 		// _indexBuffer.bind();
// 		// _indexBuffer.setCapacity(sizeof(uint32_t) * _maxIndices);

// 		_viewProjectionMatrix = Matrix3::identity;

// 		clearData();
// 	}

// 	void Renderer::terminate() {
// 		delete[] _vertexData;
// 		delete[] _indexData;
// 		delete[] _textureData;
// 		delete[] _textureIndices;

// 		_vertexData = nullptr;
// 		_indexData = nullptr;
// 		_textureData = nullptr;
// 		_textureIndices = nullptr;

// 		// _indexBuffer.destroy();
// 		// _vertexBuffer.destroy();
// 		// _vertexArray.destroy();
// 		// _batchShader.destroy();
// 	}

// 	void Renderer::setDrawMode(DrawMode mode) {
// 		glPolygonMode(GL_FRONT_AND_BACK, static_cast<int>(mode));
// 	}

// 	void Renderer::setMaxQuad(size_t max) {
// 		if (max < _circleSegment)
// 			max = _circleSegment;
// 		_maxQuad = max;
// 	}

// 	void Renderer::setCircleSegment(size_t segment) {
// 		if (segment < 3)
// 			segment = 3;
// 		if (_maxQuad < segment)
// 			_maxQuad = segment;
// 		_circleSegment = segment;
// 	}

// 	void Renderer::setViewport(Vector2 size) {
// 		glViewport(0, 0, size.x, size.y);
// 	}

// 	void Renderer::setViewProjection(const Matrix3& vp) {
// 		_viewProjectionMatrix = vp;
// 	}

// 	void Renderer::clear(const Vector4& color) {
// 		glClearColor(color.x, color.y, color.z, color.w);
// 		glClear(GL_COLOR_BUFFER_BIT);
// 	}

// 	void Renderer::checkCapacityLimit(size_t vertexOffset, size_t indexOffset, size_t textureOffset) {
// 		if (_vertexCounter + vertexOffset > _maxVertices)
// 			return render();
// 		if (_indexCounter + indexOffset > _maxIndices)
// 			return render();
// 		if (_textureCounter + textureOffset > _maxTextureUnits)
// 			return render();
// 	}

// 	void Renderer::draw(const float* vertexData, size_t vertexLength, const uint32_t* indexData, size_t indexLength) {
// 		size_t vertexOffset = _vertexCounter * Vertex::length;
// 		size_t vertexSize = vertexLength * Vertex::length;

// 		for (size_t i = 0; i < vertexSize; ++i)
// 			_vertexData[vertexOffset + i] = vertexData[i];

// 		for (size_t i = 0; i < indexLength; ++i)
// 			_indexData[_indexCounter + i] = _vertexCounter + indexData[i];

// 		_vertexCounter += vertexLength;
// 		_indexCounter += indexLength;
// 	}

// 	void Renderer::draw(const Texture& texture) {
// 		for (size_t i = 0; i < _textureCounter; ++i) {
// 			if (_textureData[i].getId() == texture.getId()) {
// 				_textureIndex = i;
// 				return;
// 			}
// 		}

// 		_textureData[_textureCounter] = texture;
// 		_textureIndex = _textureCounter;
// 		_textureCounter++;
// 	}

// 	void Renderer::clearData() {
// 		_vertexCounter = 0;
// 		_indexCounter = 0;
// 		_textureCounter = 0;
// 	}

// 	void Renderer::render() {
// 		if (_vertexCounter == 0)
// 			return;

// 		_batchShader.bind();
// 		_batchShader.setUniformMatrix3("uViewProjection", _viewProjectionMatrix);
// 		_batchShader.setUniformInt("uTex", _maxTextureUnits, _textureIndices);

// 		for (size_t i = 0; i < _textureCounter; ++i)
// 			_textureData[i].bind(i);

// 		// _vertexBuffer.bind();
// 		// _vertexBuffer.setData(sizeof(float) * _maxVertices * Vertex::length, _vertexData);

// 		// _indexBuffer.bind();
// 		// _indexBuffer.setData(sizeof(uint32_t) * _maxIndices, _indexData);

// 		_vertexArray.bind();
// 		glDrawElements(GL_TRIANGLES, _indexCounter, GL_UNSIGNED_INT, nullptr);

// 		clearData();
// 	}


// // Primitive geometries

// 	void
// 	Renderer::drawTriangle(const Matrix3& transform,
// 		const Vector2& p0,
// 		const Vector2& p1,
// 		const Vector2& p2,
// 		const Vector4& c) {
// 		checkCapacityLimit(3, 3);

// 		Vector2 l0 = p0 * transform;
// 		Vector2 l1 = p1 * transform;
// 		Vector2 l2 = p2 * transform;

// 		float vertices[] = {
// 			l0.x, l0.y, c.x, c.y, c.z, c.w, 0.f, 0.f, -1,
// 			l1.x, l1.y, c.x, c.y, c.z, c.w, 0.f, 0.f, -1,
// 			l2.x, l2.y, c.x, c.y, c.z, c.w, 0.f, 0.f, -1,
// 		};

// 		uint32_t indices[] = { 0, 1, 2 };

// 		draw(vertices, 3, indices, 3);
// 	}

// 	void Renderer::drawQuad(const Matrix3& transform,
// 		const Vector2& p0,
// 		const Vector2& p1,
// 		const Vector2& p2,
// 		const Vector2& p3,
// 		const Vector4& c) {
// 		checkCapacityLimit(4, 6);

// 		Vector2 l0 = p0 * transform;
// 		Vector2 l1 = p1 * transform;
// 		Vector2 l2 = p2 * transform;
// 		Vector2 l3 = p3 * transform;

// 		float vertices[] = {
// 			l0.x, l0.y, 	c.x, c.y, c.z, c.w, 	0.f, 0.f, 	-1,
// 			l1.x, l1.y, 	c.x, c.y, c.z, c.w, 	0.f, 0.f, 	-1,
// 			l2.x, l2.y, 	c.x, c.y, c.z, c.w, 	0.f, 0.f, 	-1,
// 			l3.x, l3.y, 	c.x, c.y, c.z, c.w, 	0.f, 0.f, 	-1,
// 		};

// 		uint32_t indices[] = {
// 			0, 1, 2,
// 			2, 3, 0 
// 		};

// 		draw(vertices, 4, indices, 6);
// 	}

// 	void
// 	Renderer::drawQuad(const Matrix3& transform, const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3) {
// 		checkCapacityLimit(4, 6);

// 		Vector2 l0 = v0.position * transform;
// 		Vector2 l1 = v1.position * transform;
// 		Vector2 l2 = v2.position * transform;
// 		Vector2 l3 = v3.position * transform;

// 		float vertices[] = {
// 			l0.x,
// 			l0.y,
// 			v0.color.x,
// 			v0.color.y,
// 			v0.color.z,
// 			v0.color.w,
// 			v0.texCoord.x,
// 			v0.texCoord.y,
// 			v0.texIndex,

// 			l1.x,
// 			l1.y,
// 			v1.color.x,
// 			v1.color.y,
// 			v1.color.z,
// 			v1.color.w,
// 			v1.texCoord.x,
// 			v1.texCoord.y,
// 			v1.texIndex,

// 			l2.x,
// 			l2.y,
// 			v2.color.x,
// 			v2.color.y,
// 			v2.color.z,
// 			v2.color.w,
// 			v2.texCoord.x,
// 			v2.texCoord.y,
// 			v2.texIndex,

// 			l3.x,
// 			l3.y,
// 			v3.color.x,
// 			v3.color.y,
// 			v3.color.z,
// 			v3.color.w,
// 			v3.texCoord.x,
// 			v3.texCoord.y,
// 			v3.texIndex,
// 		};

// 		uint32_t indices[] = {
// 			0, 1, 2,
// 			2, 3, 0 
// 		};

// 		draw(vertices, 4, indices, 6);
// 	}

// 	void Renderer::drawRectangle(const Matrix3& transform, const Vector2& p0, const Vector2& p1, const Vector4& color) {
// 		drawQuad(transform, p0 * transform, Vector2(p0.x, p1.y) * transform, p1 * transform, Vector2(p1.x, p0.y) * transform, color);
// 	}

// 	void Renderer::drawPolygon(const Matrix3& transform, const Vector2& p0, float r, size_t segments, const Vector4& c) {
// 		if (segments < 3)
// 			segments = 3;

// 		size_t triangles = segments - 2;

// 		checkCapacityLimit(segments, triangles * 3);

// 		float theta = Math::twoPi / segments;
// 		float tangetialFactor = Math::tan(theta);
// 		float radialFactor = Math::cos(theta);

// 		float x = r;
// 		float y = 0;

// 		float vertices[segments * Vertex::length];
// 		uint32_t indices[triangles * 3];

// 		for (size_t i = 0, offset = 0; i < segments; i++) {
// 			Vector2 point = Vector2(p0.x + x, p0.y + y) * transform;
// 			vertices[offset + 0] = x + point.x;
// 			vertices[offset + 1] = y + point.y;

// 			vertices[offset + 2] = c.x;
// 			vertices[offset + 3] = c.y;
// 			vertices[offset + 4] = c.z;
// 			vertices[offset + 5] = c.w;

// 			vertices[offset + 6] = 0;
// 			vertices[offset + 7] = 0;

// 			vertices[offset + 8] = -1;

// 			float tx = -y;
// 			float ty = x;

// 			x += tx * tangetialFactor;
// 			y += ty * tangetialFactor;

// 			x *= radialFactor;
// 			y *= radialFactor;

// 			offset += Vertex::length;
// 		}

// 		for (size_t i = 0, offset = 0; i < triangles; ++i) {
// 			indices[offset + 0] = 0;
// 			indices[offset + 1] = i + 1;
// 			indices[offset + 2] = i + 2;
// 			offset += 3;
// 		}

// 		draw(vertices, segments, indices, triangles * 3);
// 	}

// 	void Renderer::drawCircle(const Matrix3& transform, const Vector2& p0, float r, const Vector4& c) {
// 		drawPolygon(transform, p0, r, _circleSegment, c);
// 	}

// 	// Complex geometries

// 	void Renderer::drawLineTerminator(const Matrix3& transform,
// 		const Vector2& p0,
// 		const Vector2& p1,
// 		float w,
// 		const Vector4& c) {
// 		Vector2 p3 = (p0 - p1).normalize() * w;
// 		Vector2 t0 = (p1 - p0).getPerpendicular().normalize() * w;

// 		drawQuad(transform, p1 + t0, p3 + p0 + t0, p3 + p0 - t0, p1 - t0, c);
// 	}

// 	void Renderer::drawLineAnchor(const Matrix3& transform,
// 		const Vector2& p0,
// 		const Vector2& p1,
// 		const Vector2& p2,
// 		float w,
// 		const Vector4& c) {
// 		Vector2 i0 = Vector2::zero * transform;
// 		Vector2 l0 = p0 * transform;
// 		Vector2 l1 = p1 * transform;
// 		Vector2 l2 = p2 * transform;
// 		Vector2 t0 = (l1 - l0).getPerpendicular();
// 		Vector2 t2 = (l2 - l1).getPerpendicular();

// 		if (0 < ((l1.x - l0.x) * (l2.y - l0.y) - (l2.x - l0.x) * (l1.y - l0.y))) {
// 			t0 = -t0;
// 			t2 = -t2;
// 		}

// 		t0.setLength(w);
// 		t2.setLength(w);

// 		Vector2 u0 = (l0 + t0);
// 		Vector2 u1 = (l2 + t2);
// 		Vector2 n0 = (l0 - t0);
// 		Vector2 n1 = (l2 - t2);
// 		Vector2 c0 = (l1 + t0);
// 		Vector2 c1 = (l1 + t2);
// 		Vector2 d0 = (l1 - t0);
// 		Vector2 d1 = (l1 - t2);
// 		Vector2 e0 = ((l1 - l0).setLength(w * 2) + c0);
// 		Vector2 e1 = ((l1 - l2).setLength(w * 2) + c1);

// 		auto areLinesIntersected = [](
// 			const Vector2& p0,
// 			const Vector2& p1,
// 			const Vector2& p2,
// 			const Vector2& p3,
// 			Vector2& p4) -> bool {
// 			float denom = (p3.y - p2.y) * (p1.x - p0.x) - (p3.x - p2.x) * (p1.y - p0.y);
// 			float numea = (p3.x - p2.x) * (p0.y - p2.y) - (p3.y - p2.y) * (p0.x - p2.x);
// 			float numeb = (p1.x - p0.x) * (p0.y - p2.y) - (p1.y - p0.y) * (p0.x - p2.x);

// 			float denomAbs = Math::abs(denom);
// 			float numeaAbs = Math::abs(numea);
// 			float numebAbs = Math::abs(numeb);

// 			if (numeaAbs < Math::epsilon && numebAbs < Math::epsilon && denomAbs < Math::epsilon) {
// 				p4 = Vector2::lerp(p0, p1, 0.5);
// 				return true;
// 			}

// 			if (denomAbs < Math::epsilon)
// 				return false;

// 			float mua = numea / denom;
// 			float mub = numeb / denom;

// 			if (mua < 0 || mua > 1 || mub < 0 || mub > 1) {
// 				return false;
// 			}

// 			float muax = numea / denom;
// 			p4 = (p1 - p0) * muax;
// 			p4 += p0;
// 			return true;
// 		};

// 		bool intersected = areLinesIntersected(c0, e0, c1, e1, i0);

// #ifdef ANGGUR_DEBUG_ANCHOR_POINTS
// 		AddCircle(Matrix3(), e0, 0.01, Vector4::red);
// 		AddCircle(Matrix3(), e1, 0.01, Vector4::red);
// 		AddCircle(Matrix3(), u0, 0.01, Vector4::blue);
// 		AddCircle(Matrix3(), u1, 0.01, Vector4::blue);
// 		AddCircle(Matrix3(), n0, 0.01, Vector4::green);
// 		AddCircle(Matrix3(), n1, 0.01, Vector4::green);
// 		AddCircle(Matrix3(), c0, 0.01, Vector4::yellow);
// 		AddCircle(Matrix3(), c1, 0.01, Vector4::yellow);
// 		AddCircle(Matrix3(), d0, 0.01, Vector4::pink);
// 		AddCircle(Matrix3(), d1, 0.01, Vector4::pink);
// 		AddCircle(Matrix3(), i0, 0.01, Vector4::cyan);
// 		AddCircle(Matrix3(), l1, 0.01, Vector4::purple);
// 		return;
// #endif

// 		float vertices[] = {
// 			e0.x, e0.y, c.x, c.y, c.z, c.w, 0, 0, -1, // 0
// 			e1.x, e1.y, c.x, c.y, c.z, c.w, 0, 0, -1, // 1
// 			u0.x, u0.y, c.x, c.y, c.z, c.w, 0, 0, -1, // 2
// 			u1.x, u1.y, c.x, c.y, c.z, c.w, 0, 0, -1, // 3
// 			n0.x, n0.y, c.x, c.y, c.z, c.w, 0, 0, -1, // 4
// 			n1.x, n1.y, c.x, c.y, c.z, c.w, 0, 0, -1, // 5
// 			c0.x, c0.y, c.x, c.y, c.z, c.w, 0, 0, -1, // 6
// 			c1.x, c1.y, c.x, c.y, c.z, c.w, 0, 0, -1, // 7
// 			d0.x, d0.y, c.x, c.y, c.z, c.w, 0, 0, -1, // 8
// 			d1.x, d1.y, c.x, c.y, c.z, c.w, 0, 0, -1, // 9
// 			i0.x, i0.y, c.x, c.y, c.z, c.w, 0, 0, -1, // 10
// 			l1.x, l1.y, c.x, c.y, c.z, c.w, 0, 0, -1, // 11
// 		};

// 		uint32_t indexLength = 21;
// 		uint32_t indices[] = {
// 			2, 6, 8,
// 			8, 4, 2,
// 			7, 3, 5,
// 			5, 9, 7,
// 			6, 7, 11, // mid
// 			6, 0, 1,
// 			1, 7, 6,
// 		};

// 		if (intersected) {
// 			indexLength = 18;
// 			indices[15] = 6;
// 			indices[16] = 10;
// 			indices[17] = 7;
// 		}

// 		draw(vertices, 12, indices, indexLength);
// 	}

// 	void
// 	Renderer::drawLine(const Matrix3& transform, const Vector2& p0, const Vector2& p1, float w, const Vector4& c) {
// 		Vector2 m0 = Vector2::lerp(p0, p1, 0.5);

// 		drawLineTerminator(transform, p0, m0, w, c);
// 		drawLineTerminator(transform, p1, m0, w, c);
// 	}

// 	void Renderer::drawPolyLine(const Matrix3& transform, const std::vector<Vector2>& ps, float w, const Vector4& c) {
// 		if (ps.size() > 1) {
// 			std::vector<Vector2> ms;

// 			for (size_t i = 0; i < ps.size() - 1; ++i)
// 				ms.push_back(Vector2::lerp(ps[i], ps[i + 1], 0.5));

// 			for (size_t i = 1; i < ms.size(); ++i)
// 				drawLineAnchor(transform, ms[i - 1], ps[i], ms[i], w, c);

// 			drawLineTerminator(transform, ps.front(), ms.front(), w, c);
// 			drawLineTerminator(transform, ps.back(), ms.back(), w, c);
// 		}
// 	}

// 	void Renderer::drawPolyLineConnected(const Matrix3& transform, const std::vector<Vector2>& ps, float w, const Vector4& c) {
// 		if (ps.size() > 1) {
// 			std::vector<Vector2> ms;

// 			for (size_t i = 0; i < ps.size() - 1; ++i)
// 				ms.push_back(Vector2::lerp(ps[i], ps[i + 1], 0.5));

// 			for (size_t i = 1; i < ms.size(); ++i)
// 				drawLineAnchor(transform, ms[i - 1], ps[i], ms[i], w, c);

// 			Vector2 m = Vector2::lerp(ps.front(), ps.back(), 0.5);

// 			drawLineAnchor(transform, m, ps.front(), ms.front(), w, c);
// 			drawLineAnchor(transform, ms.back(), ps.back(), m, w, c);
// 		}
// 	}

// 	void Renderer::drawQuadraticBezier(const Matrix3& transform,
// 		const Vector2& p0,
// 		const Vector2& p1,
// 		const Vector2& p2,
// 		float w,
// 		const Vector4& c) {
// 		auto GetLerped = [](const Vector2& p0, const Vector2& p1, const Vector2& p2, float t) {
// 			Vector2 pt;

// 			float t2 = t * 2;
// 			float tq = t * t;
// 			float ti = 1.f - t;
// 			float tiq = ti * ti;

// 			pt.x = tiq * p0.x +
// 				ti * t2 * p1.x +
// 				tq * p2.x;
// 			pt.y = tiq * p0.y +
// 				ti * t2 * p1.y +
// 				tq * p2.y;

// 			return pt;
// 		};

// 		std::vector<Vector2> points;

// 		for (int i = 0; i <= 10; ++i)
// 			points.push_back(GetLerped(p0, p1, p2, i / 10.f));

// 		drawPolyLine(transform, points, w, c);
// 	}

// 	void Renderer::drawQuadraticBezierAlt(const Matrix3& transform,
// 		const Vector2& p0,
// 		const Vector2& p1,
// 		const Vector2& p2,
// 		float w,
// 		const Vector4& c) {
// 		Vector2 px = p1 * 2 - (p0 + p2) / 2;
// 		drawQuadraticBezier(transform, p0, px, p2, w, c);
// 	}

// 	void Renderer::drawQubicBezier(const Matrix3& transform,
// 		const Vector2& p0,
// 		const Vector2& p1,
// 		const Vector2& p2,
// 		const Vector2& p3,
// 		float w,
// 		const Vector4& c) {
// 		auto GetLerped = [](const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3, float t) {
// 			Vector2 pt;

// 			float t3 = t * 3;
// 			float tc = t * t * t;
// 			float ti = 1.f - t;
// 			float tiq = ti * ti;
// 			float tic = ti * ti * ti;

// 			pt.x = tic * p0.x +
// 				tiq * t3 * p1.x +
// 				ti * t3 * t * p2.x +
// 				tc * p3.x;
// 			pt.y = tic * p0.y +
// 				tiq * t3 * p1.y +
// 				ti * t3 * t * p2.y +
// 				tc * p3.y;

// 			return pt;
// 		};

// 		std::vector<Vector2> points;

// 		for (int i = 0; i <= 10; ++i)
// 			points.push_back(GetLerped(p0, p1, p2, p3, i / 10.f));

// 		drawPolyLine(transform, points, w, c);
// 	}

// // Natural geometries

// 	void Renderer::drawConvex(const Matrix3& transform, const std::vector<Vector2>& ps, const Vector4& c) {
// 		size_t triangles = ps.size() - 2;
// 		uint32_t indices[triangles * 3];
// 		float vertices[ps.size() * Vertex::length];

// 		for (size_t i = 0, offset = 0; i < ps.size(); ++i) {
// 			vertices[offset + 0] = ps[i].x;
// 			vertices[offset + 1] = ps[i].y;

// 			vertices[offset + 2] = c.x;
// 			vertices[offset + 3] = c.y;
// 			vertices[offset + 4] = c.z;
// 			vertices[offset + 5] = c.w;

// 			vertices[offset + 6] = 0;
// 			vertices[offset + 7] = 0;

// 			vertices[offset + 8] = -1;

// 			offset += Vertex::length;
// 		}

// 		for (size_t i = 0, offset = 0; i < triangles; ++i) {
// 			indices[offset + 0] = 0;
// 			indices[offset + 1] = i + 1;
// 			indices[offset + 2] = i + 2;

// 			offset += 3;
// 		}

// 		draw(vertices, ps.size(), indices, triangles * 3);
// 	}


// // Text

// 	void Renderer::drawText(const Matrix3& transform,
// 		const Vector2& p0,
// 		const Vector2& p1,
// 		const std::string& textBuffer,
// 		const TextOption& textOption,
// 		Font& textFont,
// 		const Vector4& color) {
// 		Vector2 offset = p0;
// 		Vector2 containerArea = p1 - p0;
// 		Vector2 occupiedArea;
// 		Vector2 lineArea;
// 		Vector2 wordOffset;
// 		Vector2 wordArea;

// 		occupiedArea.y = -textFont.getLineHeight() * textOption.size * textOption.lineHeight;
// 		lineArea.y = occupiedArea.y;
// 		wordArea.y = occupiedArea.y - textOption.size * 0.3;

// 		drawRectangle(transform, p0, p1, Vector4(0, 0, 1, 0.4));

// 		bool isFit = true;

// 		float wordSpace = textOption.size * textOption.wordSpace;
// 		float letterSpace = textOption.size * textOption.letterSpace;

// 		Vector2 ellipsisArea;
// 		std::vector<CodepointContainer> ellipsisCcs;

// 		for (size_t i = 0; i < textOption.ellipsis.size(); ++i) {
// 			int codepoint = textOption.ellipsis[i];
// 			CodepointContainer cc;
// 			cc.glyph = textFont.glyphs[codepoint];
// 			cc.offset.x = ellipsisArea.x;
// 			cc.offset.y = textOption.size * cc.glyph.ascent;
// 			cc.area.x = textOption.size * cc.glyph.size.x;
// 			cc.area.y = textOption.size * cc.glyph.size.y;
// 			ellipsisCcs.push_back(cc);

// 			ellipsisArea.x += cc.area.x + letterSpace;
// 		}
// 		ellipsisArea.x -= letterSpace;

// 		std::vector<CodepointContainer> ccs;

// 		auto CreateNewWord = [&]() {
// 			wordOffset.x += wordSpace;

// 			lineArea.x += wordArea.x;
// 			lineArea.x += wordSpace;
// 			wordArea.x -= letterSpace;

// 			#ifdef ANGGUR_DEBUG_TEXT_RECTS
// 			drawRectangle(transform, offset, offset + wordArea, Vector4(1, 1, 1, 0.33));
// 			#endif
			
// 			drawText(transform, offset, ccs, textFont, color);

// 			offset.x += wordOffset.x;
// 			wordOffset.set(0, 0);
// 			wordArea.x = 0;
// 			ccs.clear();
// 		};

// 		auto CreateNewLine = [&]() {
// 			offset.x = p0.x;
// 			lineArea.x -= wordSpace;
// 			lineArea.x -= letterSpace;

// 			occupiedArea.x = Math::max(occupiedArea.x, lineArea.x);
// 			occupiedArea.y += lineArea.y;
// 			#ifdef ANGGUR_DEBUG_TEXT_RECTS
// 			drawRectangle(transform, offset, offset + lineArea, Vector4(1, 1, 1, 0.33));
// 			#endif

// 			lineArea.x = 0;
// 			offset.y += lineArea.y;
// 		};

// 		for (size_t i = 0; i < textBuffer.size(); ++i) {
// 			int codepoint = textBuffer[i];
// 			CodepointContainer cc;
// 			cc.glyph = textFont.glyphs[codepoint];
// 			cc.offset.x = wordOffset.x;
// 			cc.offset.y = textOption.size * cc.glyph.ascent;
// 			cc.area.x = textOption.size * cc.glyph.size.x;
// 			cc.area.y = textOption.size * cc.glyph.size.y;
// 			ccs.push_back(cc);

// 			if (codepoint == '\n') {
// 				CreateNewWord();
// 				CreateNewLine();
// 			}
// 			else if (codepoint == ' ') {
// 				if (lineArea.x + wordArea.x > containerArea.x) {

// 					if (occupiedArea.y + lineArea.y > containerArea.y) {
// 						occupiedArea.x = Math::max(occupiedArea.x, lineArea.x);
// 						#ifdef ANGGUR_DEBUG_TEXT_RECTS
// 						drawCircle(transform,
// 							p0 + Vector2(occupiedArea.x, occupiedArea.y + lineArea.y),
// 							0.1,
// 							Vector4(1, 1, 0, 0.5));
// 						#endif
// 						isFit = false;
// 						drawText(transform, offset, ellipsisCcs, textFont, color);
// 						break;
// 					}
// 					else {
// 						CreateNewLine();
// 						CreateNewWord();
// 					}
// 				}
// 				else
// 					CreateNewWord();
// 			}
// 			else {
// 				// TODO: Split the word if longer than the container area

// 				float x = letterSpace + cc.area.x + textFont.getKerning(codepoint, textBuffer[i + 1]) * textOption.size;
// 				wordOffset.x += x;
// 				wordArea.x += x;
// 			}
// 		}

// 		if (isFit) {
// 			CreateNewLine();
// 			CreateNewWord();
// 		}

// 		offset.x = p0.x;
// 		#ifdef ANGGUR_DEBUG_TEXT_RECTS
// 		drawRectangle(transform, offset, offset + lineArea, Vector4(1, 1, 1, 0.33));
// 		drawRectangle(transform, p0, p0 + occupiedArea, Vector4(1, 1, 1, 0.33));
// 		drawCircle(transform, p0 + containerArea, 0.1, Vector4::green);
// 		#endif
// 	}

// 	void Renderer::drawText(const Matrix3& transform,
// 		const Vector2& p0,
// 		const std::vector<CodepointContainer>& ccs,
// 		Font& textFont,
// 		const Vector4& color) {
		
// 		for (const CodepointContainer& cc: ccs) {
// 			#ifdef ANGGUR_DEBUG_TEXT_RECTS
// 			drawRectangle(transform, p0 + cc.offset, p0 + cc.offset + cc.area, Vector4(1, 1, 1, 0.33));
// 			#endif
			
// 			drawTexturedQuad(
// 				Vector2(p0.x + cc.offset.x, p0.y + cc.offset.y) * transform,
// 				Vector2(p0.x + cc.offset.x + cc.area.x, p0.y + cc.offset.y) * transform,
// 				Vector2(p0.x + cc.offset.x + cc.area.x, p0.y + cc.offset.y + cc.area.y) * transform,
// 				Vector2(p0.x + cc.offset.x, p0.y + cc.offset.y + cc.area.y) * transform,

// 				Vector2(cc.glyph.x, cc.glyph.y),
// 				Vector2(cc.glyph.x + cc.glyph.w, cc.glyph.y),
// 				Vector2(cc.glyph.x + cc.glyph.w, cc.glyph.y + cc.glyph.h),
// 				Vector2(cc.glyph.x, cc.glyph.y + cc.glyph.h),
// 				textFont.getTexture(),
// 				color
// 			);
// 		}
// 	}

// 	// Textured geometries

// 	void Renderer::drawTexturedQuad(const Vector2& p0,
// 		const Vector2& p1,
// 		const Vector2& p2,
// 		const Vector2& p3,
// 		const Vector2& t0,
// 		const Vector2& t1,
// 		const Vector2& t2,
// 		const Vector2& t3,
// 		const Texture& t,
// 		const Vector4& c) {
// 		checkCapacityLimit(4, 6, 1);

// 		draw(t);
// 		float ti = _textureIndex;
// 		if (t.getChannels() == 1)
// 			ti += _maxTextureUnits;

// 		float vertices[] = {
// 			p0.x, p0.y, 	c.x, c.y, c.z, c.w, 	t0.x, t0.y, 	ti,
// 			p1.x, p1.y, 	c.x, c.y, c.z, c.w, 	t1.x, t1.y, 	ti,
// 			p2.x, p2.y, 	c.x, c.y, c.z, c.w, 	t2.x, t2.y, 	ti,
// 			p3.x, p3.y, 	c.x, c.y, c.z, c.w, 	t3.x, t3.y, 	ti,
// 		};

// 		uint32_t indices[] = {
// 			0, 1, 2,
// 			2, 3, 0,
// 		};

// 		draw(vertices, 4, indices, 6);
// 	}

// 	void Renderer::drawTexture(const Vector2& p0, const Texture& t, const Vector4& c) {
// 		float w = t.getWidth();
// 		float h = t.getHeight();

// 		drawTexturedQuad(
// 			p0,
// 			{ p0.x + w, p0.y },
// 			{ p0.x + w, p0.y + h },
// 			{ p0.x, p0.y + h },
// 			{ 0.f, 0.f },
// 			{ 1.f, 0.f },
// 			{ 1.f, 1.f },
// 			{ 0.f, 1.f },
// 			t,
// 			c);
// 	}

// 	void Renderer::drawTexturedRectangle(const Vector2& p0, float w, float h, const Texture& t, const Vector4& c) {
// 		drawTexturedQuad(
// 			p0,
// 			{ p0.x + w, p0.y },
// 			{ p0.x + w, p0.y + h },
// 			{ p0.x, p0.y + h },
// 			{ 0, 0 },
// 			{ 1, 0 },
// 			{ 1, 1 },
// 			{ 0, 1 },
// 			t,
// 			c);
// 	}
// } 
