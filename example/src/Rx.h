#pragma once

#include <Anggur/Core/Application.h>
#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Texture.h>
#include <Anggur/Utility/Log.h>
#include <memory>

using namespace Anggur;

class Rx {
public:
	static void initialize();

	static void beginScene();
	static void endScene();

	static void update();

private:
	static void initializeVertexArray();
	static void initializeShader();

private:
	static std::unique_ptr<Shader> shader;
	static std::unique_ptr<VertexArray> vertexArray;
	static std::unique_ptr<VertexBuffer> vertexBuffer;
	static std::unique_ptr<IndexBuffer> indexBuffer;
	static std::vector<float> vertices;
	static std::vector<uint32_t> indices;
};