#include "anggur/graphics/renderer.h"
#include "anggur/graphics/api.h"
#include "anggur/graphics/shader.h"
#include "anggur/graphics/vertexbuffer.h"
#include "anggur/math/vector2.h"
#include <iostream>
#include <vector>

namespace Anggur {
	Renderer::Renderer() {
		type = RendererType::circle;

		std::vector<unsigned char> pixels = {0xFF, 0xFF, 0xFF};
		defaultTexture = new Texture(pixels, 1, 1, 3);
	}

	Renderer::~Renderer() {
		delete defaultTexture;
	}

	void Renderer::flush() {
		switch (type) {
			case RendererType::triangle:
				return triangleRenderer.flush();

			case RendererType::circle:
				return circleRenderer.flush();

			case RendererType::roundedRectangle:
				return rectangleRenderer.flush();

			case RendererType::line:
				return lineRenderer.flush();

			case RendererType::text:
				return textRenderer.flush();
		}

		++drawCount;
	}

	void Renderer::clear(const Vector4& color) {
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Renderer::setViewport(const Vector2& position, const Vector2& size) {
		glViewport(position.x, position.y, size.x, size.y);
	}

	void Renderer::setView(const Matrix3& newView) {
		triangleRenderer.setView(newView);
		rectangleRenderer.setView(newView);
		circleRenderer.setView(newView);
		lineRenderer.setView(newView);
		textRenderer.setView(newView);
	}

	void Renderer::beginScene() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		drawCount = 0;
	}

	void Renderer::endScene() {
		flush();
	}

	void Renderer::beginMask() {
		flush();

		if (stencilDepth == 0)
			glEnable(GL_STENCIL_TEST);

		++stencilDepth;
	}

	void Renderer::beginWriteMask() {
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_ALWAYS, stencilDepth, stencilDepth);
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	}

	void Renderer::endWriteMask() {
		flush();

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilFunc(GL_EQUAL, stencilDepth, stencilDepth);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}

	void Renderer::beginEraseMask() {
		flush();

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_ALWAYS, stencilDepth, stencilDepth);
		glStencilOp(GL_DECR, GL_DECR, GL_DECR);
	}

	void Renderer::endEraseMask() {
		flush();

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
	}

	void Renderer::endMask() {
		flush();

		--stencilDepth;

		if (stencilDepth == 0)
			glDisable(GL_STENCIL_TEST);
	}

	void Renderer::setType(RendererType newType) {
		if (type != newType) {
			flush();
			type = newType;
		}
	}

	void Renderer::drawTriangle(
		const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color
	) {
		setType(RendererType::triangle);

		triangleRenderer.addTriangle(
			position0, position1, position2, color, defaultTexture, Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f),
			Vector2(1.0f, 1.0f)
		);
	}

	void Renderer::drawTriangleTexture(
		const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector4& color,
		Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
		const Vector2& texturePosition2
	) {
		setType(RendererType::triangle);

		triangleRenderer.addTriangle(
			position0, position1, position2, color, texture, texturePosition0, texturePosition1, texturePosition2
		);
	}

	void Renderer::drawQuad(
		const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3,
		const Vector4& color
	) {
		setType(RendererType::triangle);

		triangleRenderer.addQuad(
			position0, position1, position2, position3, color, defaultTexture, Vector2(0.0f, 0.0f),
			Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f), Vector2(0.0f, 1.0f)
		);
	}

	void Renderer::drawQuadTexture(
		const Vector2& position0, const Vector2& position1, const Vector2& position2, const Vector2& position3,
		const Vector4& color, Texture* texture, const Vector2& texturePosition0, const Vector2& texturePosition1,
		const Vector2& texturePosition2, const Vector2& texturePosition3
	) {
		setType(RendererType::triangle);

		triangleRenderer.addQuad(
			position0, position1, position2, position3, color, texture, texturePosition0, texturePosition1,
			texturePosition2, texturePosition3
		);
	}

	void Renderer::drawRectangle(const Vector2& position, const Vector2& size, const Vector4& color) {
		setType(RendererType::triangle);

		triangleRenderer.addRectangle(
			position, size, color, defaultTexture, Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f)
		);
	}

	void Renderer::drawRectangleTexture(
		const Vector2& position, const Vector2& size, const Vector4& color, Texture* texture,
		const Vector2& texturePosition, const Vector2& textureSize
	) {
		setType(RendererType::triangle);

		triangleRenderer.addRectangle(position, size, color, texture, texturePosition, textureSize);
	}

	void Renderer::drawRoundedRectangle(
		const Vector2& position, const Vector2& size, float radius, float thickness, float sharpness,
		const Vector4& color
	) {
		setType(RendererType::roundedRectangle);

		rectangleRenderer.add(position, size, radius, thickness, sharpness, color);
	}

	void Renderer::drawCircle(
		const Vector2& position, float radius, float thickness, float sharpness, const Vector4& color
	) {
		setType(RendererType::circle);

		circleRenderer.add(position, radius, thickness, sharpness, color);
	}

	void Renderer::drawLine(
		const Vector2& positionA, const Vector2& positionB, float thickness, float sharpness, const Vector4& color
	) {
		setType(RendererType::line);

		lineRenderer.add(positionA, positionB, thickness, sharpness, color);
	}

	void Renderer::drawCharacter(
		const Vector2& position, unsigned int character, Font* font, float size, float thickness, float sharpness,
		const Vector4& color
	) {
		setType(RendererType::text);

		textRenderer.add(position, character, font, size, thickness, sharpness, color);
	}

	void Renderer::drawText(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		const Vector4& color
	) {
		setType(RendererType::text);

		textRenderer.add(position, content, font, size, thickness, sharpness, color);
	}

	void Renderer::drawTextLine(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		const Vector4& color
	) {
		setType(RendererType::text);

		textRenderer.addLine(position, content, font, size, thickness, sharpness, color);
	}

	void Renderer::drawTextLineCut(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		float limit, const Vector4& color
	) {
		setType(RendererType::text);

		textRenderer.addLineCut(position, content, font, size, thickness, sharpness, limit, color);
	}

	void Renderer::drawTextFlow(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		float limit, const Vector4& color
	) {
		setType(RendererType::text);

		textRenderer.addFlow(position, content, font, size, thickness, sharpness, limit, color);
	}

	void Renderer::drawTextFlowCut(
		const Vector2& position, const std::string& content, Font* font, float size, float thickness, float sharpness,
		const Vector2& limit, const Vector4& color
	) {
		setType(RendererType::text);

		textRenderer.addFlowCut(position, content, font, size, thickness, sharpness, limit, color);
	}

	Vector2 Renderer::measureText(
		const std::string& content, Font* font, float size, float thickness, float sharpness
	) {
		return textRenderer.measure(content, font, size, thickness, sharpness);
	}

	Vector2 Renderer::measureTextFlow(
		const std::string& content, Font* font, float size, float thickness, float sharpness, float limit
	) {
		return textRenderer.measureFlow(content, font, size, thickness, sharpness, limit);
	}
}
