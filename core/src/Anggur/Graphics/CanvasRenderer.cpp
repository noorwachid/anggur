#include "Function.h"
#include "VertexArray.h"
#include "Shader.h"
#include "CanvasRenderer.h"
#include "Anggur/Math/Vector2.h"
#include <vector>
#include <iostream>

namespace Anggur 
{
    CanvasRenderer::CanvasRenderer() {
        initializeVertexPool();
        initializeTexturePool();
        initializeShader();
    }

    CanvasRenderer::~CanvasRenderer() {
    }

    void CanvasRenderer::initializeVertexPool() {
        vertices.assign(batchVertex, CanvasVertex(Vector2(0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f), 0.0f));
        indices.assign(batchVertex * batchIndexMultiplier, 0);

        vertexBuffer = std::make_shared<VertexBuffer>();
        vertexBuffer->setCapacity(sizeof(CanvasVertex) * vertices.size());

        vertexArray = std::make_shared<VertexArray>();
        vertexArray->setAttribute(0, 2, GL_FLOAT, sizeof(CanvasVertex), offsetof(CanvasVertex, position));
        vertexArray->setAttribute(1, 4, GL_FLOAT, sizeof(CanvasVertex), offsetof(CanvasVertex, color));
        vertexArray->setAttribute(2, 2, GL_FLOAT, sizeof(CanvasVertex), offsetof(CanvasVertex, uv));
        vertexArray->setAttribute(3, 1, GL_FLOAT, sizeof(CanvasVertex), offsetof(CanvasVertex, slot));
        
        indexBuffer = std::make_shared<IndexBuffer>();
        indexBuffer->setCapacity(sizeof(uint32_t) * indices.size());
    }

    void CanvasRenderer::initializeTexturePool() {
        textures.assign(Texture::getMaxSlot(), nullptr);
        slots.reserve(Texture::getMaxSlot());

        for (int i = 0; i < Texture::getMaxSlot(); ++i) {
            slots.push_back(i);
        }

        uint8_t whitePixel[] = {255, 255, 255, 255};
        whiteTexture = std::make_shared<Texture2D>(whitePixel, 1, 1, 4);
    }

    void CanvasRenderer::initializeShader() {
        shader = std::make_shared<Shader>();
        shader->setVertexSource(R"(
            #version 330 core

            layout (location = 0) in vec2 aPosition;
            layout (location = 1) in vec4 aColor;
            layout (location = 2) in vec2 aUV;
            layout (location = 3) in float aSlot;

            out vec4 vColor;
            out vec2 vUV;
            out float vSlot;

            uniform mat3 uViewProjection;

            void main() {
                gl_Position = vec4(uViewProjection * vec3(aPosition, 1.0f), 1.0f);

                vColor = aColor;
                vUV = aUV;
                vSlot = aSlot;
            }
        )");

        shader->setFragmentSource(R"(
            #version 330 core
            
            in vec4 vColor;
            in vec2 vUV;
            in float vSlot;

            out vec4 fColor;

            uniform sampler2D uSlots[)" + std::to_string(Texture::getMaxSlot()) + R"(];
            
            void main() {
                fColor = texture(uSlots[int(vSlot)], vUV) * vColor;
            }
        )");

        shader->compile();
    }

    void CanvasRenderer::setBatchChunk(size_t vertex, size_t indexMultiplier) {
        batchVertex = vertex;
        batchIndexMultiplier = indexMultiplier;
    }

    void CanvasRenderer::clear(const Vector4& color) {
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void CanvasRenderer::setViewport(const Vector2& size) {
        glViewport(0, 0, size.x, size.y);
    }

    void CanvasRenderer::setViewport(const Vector2& position, const Vector2& size) {
        glViewport(position.x, position.y, size.x, size.y);
    }

    void CanvasRenderer::setViewProjection(const Matrix3& newViewProjection) {
        viewProjection = newViewProjection;
    }

    void CanvasRenderer::begin() {
        renderCount = 0;

        flushInternalBuffer();
    }

    void CanvasRenderer::begin(const Matrix3& viewProjection) {
        setViewProjection(viewProjection);
        begin();
    }

    void CanvasRenderer::end() {
        flush();
    }

    void CanvasRenderer::flush() {
        if (vertexOffset == 0) 
        {
            return;
        }

        for (size_t i = 0; i < textureOffset; ++i) 
        {
            textures[i]->bind(i);
        }

        shader->bind();
        shader->setUniformMatrix3("uViewProjection", viewProjection);
        shader->setUniformInt("uSlots", textureOffset, slots.data());

        vertexArray->bind();
        
        vertexBuffer->bind();
        vertexBuffer->setData(sizeof(CanvasVertex) * vertexOffset, vertices.data());

        indexBuffer->bind();
        indexBuffer->setData(sizeof(uint32_t) * indexOffset, indices.data());

        glDrawElements(GL_TRIANGLES, indexOffset, GL_UNSIGNED_INT, nullptr);

        flushInternalBuffer();

        ++renderCount;
    }

    void CanvasRenderer::flushInternalBuffer() {
        vertexOffset = 0;
        indexOffset = 0;
        textureOffset = 0;
    }

    bool CanvasRenderer::isCapacityMaxout(size_t newVertexSize, size_t newIndexSize, size_t newTextureSize) {
        return 
            vertexOffset + newVertexSize > vertices.size() ||
            indexOffset + newIndexSize > indices.size() ||
            textureOffset + newTextureSize > textures.size();
    }

    void CanvasRenderer::draw(const std::vector<CanvasVertex>& newVertices, const std::vector<uint32_t>& newIndices, const std::shared_ptr<Texture2D>& texture) {
        if (isCapacityMaxout(newVertices.size(), newIndices.size(), 1)) 
            flush();

        // Find or add new texture slot 
        int textureSlot = 0;

        // This code only create one branch
        for (; textureSlot < textureOffset && textures[textureSlot]->getID() != texture->getID(); ++textureSlot);

        if (textureSlot == textureOffset) {
            textureSlot = textureOffset;
            textures[textureOffset] = texture;
            textureOffset += 1;
        }

        for (size_t i = 0; i < newVertices.size(); ++i) {
            auto& vertex = vertices[i + vertexOffset];

            vertex = newVertices[i];
            vertex.slot = textureSlot;
        }

        for (size_t i = 0; i < newIndices.size(); ++i) {
            indices[i + indexOffset] = newIndices[i] + vertexOffset;
        }

        vertexOffset += newVertices.size();
        indexOffset += newIndices.size();
    }

    // 2D primitives

    void CanvasRenderer::drawTriangle(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& point2, const Vector4& color) {
        draw(
            { 
                CanvasVertex(model * point0, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * point1, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * point2, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
            }, {
                0, 1, 2,
            }, 
            whiteTexture
        );
    }

    void CanvasRenderer::drawQuad(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector2& point2, const Vector2& point3, const Vector4& color) {
        draw(
            {     
                CanvasVertex(model * point0, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * point1, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * point2, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * point3, Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
            }, {
                0, 1, 2,
                2, 3, 0,
            }, 
            whiteTexture
        );
    }


    void CanvasRenderer::drawRectangle(const Matrix3& model, const Vector2& point0, const Vector2& point1, const Vector4& color) {
        draw(
            {    
                CanvasVertex(model * point0,                      Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * Vector2(point1.x, point0.y), Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * point1,                      Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
                CanvasVertex(model * Vector2(point0.x, point1.y), Vector4(color.x, color.y, color.z, color.w), Vector2(0, 0)),
            }, {
                0, 1, 2,
                2, 3, 0,
            }, 
            whiteTexture
        );
    }

    void CanvasRenderer::drawTexturedRectangle(const Matrix3& model, const Vector2& point0, const Vector2& point1, const std::shared_ptr<Texture2D>& texture, const Vector2& texturePoint0, const Vector2& texturePoint1, const Vector4& color) {
        draw(
            {    
                CanvasVertex(model * point0,                      Vector4(color.x, color.y, color.z, color.w), texturePoint0),
                CanvasVertex(model * Vector2(point1.x, point0.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePoint1.x, texturePoint0.y)),
                CanvasVertex(model * point1,                      Vector4(color.x, color.y, color.z, color.w), texturePoint1),
                CanvasVertex(model * Vector2(point0.x, point1.y), Vector4(color.x, color.y, color.z, color.w), Vector2(texturePoint0.x, texturePoint1.y)),
            }, {
                0, 1, 2,
                2, 3, 0,
            }, 
            texture
        );
    }

    void CanvasRenderer::drawCircle(const Matrix3& model, float radius, int segment, const Vector4& color) {
        if (segment < 3)
            segment = 3;

        Vector2 position(0, 0);
        size_t triangleSize = segment - 2;

        float theta = (Math::twoPi / segment);
        float tangetialFactor = Math::tan(theta);
        float radialFactor = Math::cos(theta);

        float x = radius;
        float y = 0;

        std::vector<CanvasVertex> vertices;
        std::vector<uint32_t> indices;

        vertices.reserve(segment);
        indices.reserve(triangleSize * 3);

        for (size_t i = 0; i < segment; i++) {
            CanvasVertex vertex;
            vertex.position = model * Vector2(position.x + x, position.y + y);
            // vertex.textureCoord = Vector2(x / length, y / length);
            vertex.color = color;

            float tx = -y;
            float ty = x;

            x += tx * tangetialFactor;
            y += ty * tangetialFactor;

            x *= radialFactor;
            y *= radialFactor;

            vertices.push_back(std::move(vertex));
        }

        for (size_t i = 0, offset = 0; i < triangleSize; ++i) {
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
            offset += 3;
        }

        draw(vertices, indices, whiteTexture);
    }


    void CanvasRenderer::drawArc(const Matrix3& model, float radius, float beginAngle, float sweepAngle, int segment, const Vector4& color) {
        if (segment < 3)
            segment = 3;

        Vector2 position(0, 0);

        // // TODO: simplify this equation
        float theta = Math::twoPi / ((segment - 1) / (sweepAngle / Math::twoPi));

        float tangetialFactor = Math::tan(theta);
        float radialFactor = Math::cos(theta);

        Vector2 walker = Vector2::createPolar(radius, beginAngle);

        std::vector<CanvasVertex> vertices;
        std::vector<uint32_t> indices;

        vertices.reserve(segment + 1);
        indices.reserve(segment);

        CanvasVertex vertex;
        vertex.position = model * position;
        vertex.color = color;
        vertices.push_back(vertex);

        for (size_t i = 0; i < segment; i++) {
            CanvasVertex vertex;
            vertex.position = model * Vector2(position.x + walker.x, position.y + walker.y);
            vertex.color = color;

            float tx = -walker.y;
            float ty = walker.x;

            walker.x += tx * tangetialFactor;
            walker.y += ty * tangetialFactor;

            walker.x *= radialFactor;
            walker.y *= radialFactor;

            vertices.push_back(std::move(vertex));
        }

        for (size_t i = 0; i < segment - 1; ++i) {
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
        }

        indices.push_back(0);
        indices.push_back(segment);
        indices.push_back(1);

        draw(vertices, indices, whiteTexture);
    }

    // 2D lines

	void CanvasRenderer::drawLineTerminator(const Matrix3& model, const Vector2& point0, const Vector2& point1, float thickness, const Vector4& color) {
		Vector2 offsetPoint = thickness * Vector2::normalize((point0 - point1));
		Vector2 perpenPoint = thickness * Vector2::normalize((point1 - point0).getPerpendicular());

		drawQuad(model, 
            point1 + perpenPoint, 
            point0 + perpenPoint + offsetPoint, 
            point0 - perpenPoint + offsetPoint, 
            point1 - perpenPoint, 
            color
        );
	}

	void CanvasRenderer::drawLineAnchor(const Matrix3& transform,
		const Vector2& p0,
		const Vector2& p1,
		const Vector2& p2,
		float w,
		const Vector4& c) {
		Vector2 i0 = transform * Vector2::zero;
		Vector2 l0 = transform * p0;
		Vector2 l1 = transform * p1;
		Vector2 l2 = transform * p2;
		Vector2 t0 = (l1 - l0).getPerpendicular();
		Vector2 t2 = (l2 - l1).getPerpendicular();

		if (0 < ((l1.x - l0.x) * (l2.y - l0.y) - (l2.x - l0.x) * (l1.y - l0.y))) {
			t0 = -t0;
			t2 = -t2;
		}

		t0.setLength(w);
		t2.setLength(w);

		Vector2 u0 = (l0 + t0);
		Vector2 u1 = (l2 + t2);
		Vector2 n0 = (l0 - t0);
		Vector2 n1 = (l2 - t2);
		Vector2 c0 = (l1 + t0);
		Vector2 c1 = (l1 + t2);
		Vector2 d0 = (l1 - t0);
		Vector2 d1 = (l1 - t2);
        Vector2 e0c = (l1 - l0);
        e0c.setLength(w * 2);
		Vector2 e0 = (e0c + c0);
        Vector2 e1c = (l1 - l2);
        e1c.setLength(w * 2);
		Vector2 e1 = (e1c + c1);

		auto areLinesIntersected = [](
			const Vector2& p0,
			const Vector2& p1,
			const Vector2& p2,
			const Vector2& p3,
			Vector2& p4) -> bool {
			float denom = (p3.y - p2.y) * (p1.x - p0.x) - (p3.x - p2.x) * (p1.y - p0.y);
			float numea = (p3.x - p2.x) * (p0.y - p2.y) - (p3.y - p2.y) * (p0.x - p2.x);
			float numeb = (p1.x - p0.x) * (p0.y - p2.y) - (p1.y - p0.y) * (p0.x - p2.x);

			float denomAbs = Math::abs(denom);
			float numeaAbs = Math::abs(numea);
			float numebAbs = Math::abs(numeb);

			if (numeaAbs < Math::epsilon && numebAbs < Math::epsilon && denomAbs < Math::epsilon) {
				p4 = Vector2::lerp(p0, p1, 0.5);
				return true;
			}

			if (denomAbs < Math::epsilon)
				return false;

			float mua = numea / denom;
			float mub = numeb / denom;

			if (mua < 0 || mua > 1 || mub < 0 || mub > 1) {
				return false;
			}

			float muax = numea / denom;
			p4 = muax * (p1 - p0);
			p4 = p4 + p0;
			return true;
		};

		bool intersected = areLinesIntersected(c0, e0, c1, e1, i0);

        std::vector<CanvasVertex> vertices = {
			CanvasVertex{{e0.x, e0.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 0
			CanvasVertex{{e1.x, e1.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 1
			CanvasVertex{{u0.x, u0.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 2
			CanvasVertex{{u1.x, u1.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 3
			CanvasVertex{{n0.x, n0.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 4
			CanvasVertex{{n1.x, n1.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 5
			CanvasVertex{{c0.x, c0.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 6
			CanvasVertex{{c1.x, c1.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 7
			CanvasVertex{{d0.x, d0.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 8
			CanvasVertex{{d1.x, d1.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 9
			CanvasVertex{{i0.x, i0.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 10
			CanvasVertex{{l1.x, l1.y}, {c.x, c.y, c.z, c.w}, {0, 0}}, // 11
		};

		std::vector<uint32_t> indices = {
			2, 6, 8,
			8, 4, 2,
			7, 3, 5,
			5, 9, 7,
			6, 7, 11, // mid
			6, 0, 1,
			1, 7, 6,
		};

		if (intersected) {
			indices.resize(18);
			indices[15] = 6;
			indices[16] = 10;
			indices[17] = 7;
		}

		draw(vertices, indices, whiteTexture);
	}

	void CanvasRenderer::drawLine(const Matrix3& model, const Vector2& point0, const Vector2& point1, float thickness, const Vector4& color) {
		Vector2 midPoint = Vector2::lerp(point0, point1, 0.5);

		drawLineTerminator(model, point0, midPoint, thickness, color);
		drawLineTerminator(model, point1, midPoint, thickness, color);
	}

	void CanvasRenderer::drawPolyLine(const Matrix3& transform, const std::vector<Vector2>& ps, float w, const Vector4& c) {
		if (ps.size() > 1) {
			std::vector<Vector2> ms;

			for (size_t i = 0; i < ps.size() - 1; ++i)
				ms.push_back(Vector2::lerp(ps[i], ps[i + 1], 0.5));

			for (size_t i = 1; i < ms.size(); ++i)
				drawLineAnchor(transform, ms[i - 1], ps[i], ms[i], w, c);

			drawLineTerminator(transform, ps.front(), ms.front(), w, c);
			drawLineTerminator(transform, ps.back(), ms.back(), w, c);
		}
	}

	// void CanvasRenderer::drawPolyLineConnected(const Matrix3& transform, const std::vector<Vector2>& ps, float w, const Vector4& c) {
	// 	if (ps.size() > 1) {
	// 		std::vector<Vector2> ms;

	// 		for (size_t i = 0; i < ps.size() - 1; ++i)
	// 			ms.push_back(Vector2::lerp(ps[i], ps[i + 1], 0.5));

	// 		for (size_t i = 1; i < ms.size(); ++i)
	// 			drawLineAnchor(transform, ms[i - 1], ps[i], ms[i], w, c);

	// 		Vector2 m = Vector2::lerp(ps.front(), ps.back(), 0.5);

	// 		drawLineAnchor(transform, m, ps.front(), ms.front(), w, c);
	// 		drawLineAnchor(transform, ms.back(), ps.back(), m, w, c);
	// 	}
	// }

	// void CanvasRenderer::drawQuadraticBezier(const Matrix3& transform,
	// 	const Vector2& p0,
	// 	const Vector2& p1,
	// 	const Vector2& p2,
	// 	float w,
	// 	const Vector4& c) {
	// 	auto GetLerped = [](const Vector2& p0, const Vector2& p1, const Vector2& p2, float t) {
	// 		Vector2 pt;

	// 		float t2 = t * 2;
	// 		float tq = t * t;
	// 		float ti = 1.f - t;
	// 		float tiq = ti * ti;

	// 		pt.x = tiq * p0.x +
	// 			ti * t2 * p1.x +
	// 			tq * p2.x;
	// 		pt.y = tiq * p0.y +
	// 			ti * t2 * p1.y +
	// 			tq * p2.y;

	// 		return pt;
	// 	};

	// 	std::vector<Vector2> points;

	// 	for (int i = 0; i <= 10; ++i)
	// 		points.push_back(GetLerped(p0, p1, p2, i / 10.f));

	// 	drawPolyLine(transform, points, w, c);
	// }

	// void CanvasRenderer::drawQuadraticBezierAlt(const Matrix3& transform,
	// 	const Vector2& p0,
	// 	const Vector2& p1,
	// 	const Vector2& p2,
	// 	float w,
	// 	const Vector4& c) {
	// 	Vector2 px = p1 * 2 - (p0 + p2) / 2;
	// 	drawQuadraticBezier(transform, p0, px, p2, w, c);
	// }

	// void CanvasRenderer::drawQubicBezier(const Matrix3& transform,
	// 	const Vector2& p0,
	// 	const Vector2& p1,
	// 	const Vector2& p2,
	// 	const Vector2& p3,
	// 	float w,
	// 	const Vector4& c) {
	// 	auto GetLerped = [](const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3, float t) {
	// 		Vector2 pt;

	// 		float t3 = t * 3;
	// 		float tc = t * t * t;
	// 		float ti = 1.f - t;
	// 		float tiq = ti * ti;
	// 		float tic = ti * ti * ti;

	// 		pt.x = tic * p0.x +
	// 			tiq * t3 * p1.x +
	// 			ti * t3 * t * p2.x +
	// 			tc * p3.x;
	// 		pt.y = tic * p0.y +
	// 			tiq * t3 * p1.y +
	// 			ti * t3 * t * p2.y +
	// 			tc * p3.y;

	// 		return pt;
	// 	};

	// 	std::vector<Vector2> points;

	// 	for (int i = 0; i <= 10; ++i)
	// 		points.push_back(GetLerped(p0, p1, p2, p3, i / 10.f));

	// 	drawPolyLine(transform, points, w, c);
	// }
}