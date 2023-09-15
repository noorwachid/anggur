#include "anggur/graphics/shader.h"
#include "anggur/graphics/api.h"
#include "anggur/graphics/texture.h"
#include "anggur/math/matrix3.h"
#include "anggur/math/matrix4.h"
#include "anggur/math/vector2.h"
#include "anggur/math/vector3.h"
#include "anggur/math/vector4.h"

#include <cassert>
#include <regex>
#include <stdexcept>

namespace Anggur {
	void ReplaceAll(std::string& source, const std::string& from, const std::string& to) {
		std::string newString;
		newString.reserve(source.length()); // avoids a few memory allocations

		std::string::size_type lastPos = 0;
		std::string::size_type findPos;

		while (std::string::npos != (findPos = source.find(from, lastPos))) {
			newString.append(source, lastPos, findPos - lastPos);
			newString += to;
			lastPos = findPos + from.length();
		}

		// Care for the rest after last occurrence
		newString += source.substr(lastPos);

		source.swap(newString);
	}

	Shader::Shader() {
	}

	Shader::~Shader() {
		terminate();
	}

	std::string GenerateTextureSlot(const std::string& source) {
		std::string sourceSlot = "switch ($3) {\n";

		for (int i = 0; i < TextureSpecification::getMaxSlot(); ++i) {
			std::string index = std::to_string(i);
			sourceSlot += "	case " + index + ": $1 = texture($2[" + index + "], $4); break;\n";
		}

		sourceSlot += "}\n";

		return std::regex_replace(
			source, std::regex(R"(TEXTURE_SLOT_INDEXING\(([a-zA-Z_]+), ([a-zA-Z_]+), ([a-zA-Z_]+), ([a-zA-Z_]+)\))"),
			sourceSlot
		);
	}

	void Shader::setVertexSource(const std::string& source) {
#ifdef EMSCRIPTEN
		std::string header = "#version 300 es\n";
#else
		std::string header = "#version 330 core\n";
#endif

		header += "#define TEXTURE_SLOT " + std::to_string(TextureSpecification::getMaxSlot()) + "\n";

		_vertexSource = header + source;

		_vertexSource = GenerateTextureSlot(_vertexSource);
	}

	void Shader::setFragmentSource(const std::string& source) {
#ifdef EMSCRIPTEN
		std::string header = "#version 300 es\nprecision highp float;\n";
#else
		std::string header = "#version 330 core\n";
#endif

		header += "#define TEXTURE_SLOT " + std::to_string(TextureSpecification::getMaxSlot()) + "\n";

		_fragmentSource = header + source;

		_fragmentSource = GenerateTextureSlot(_fragmentSource);
	}

	void Shader::compile() {
		const char* cVertexSource = _vertexSource.c_str();
		const char* cFragmentSource = _fragmentSource.c_str();
		int isSucceed;
		char message[512];

		unsigned int vertexId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexId, 1, &cVertexSource, NULL);
		glCompileShader(vertexId);

		glGetShaderiv(vertexId, GL_COMPILE_STATUS, &isSucceed);
		if (!isSucceed) {
			glGetShaderInfoLog(vertexId, 512, NULL, message);

			throw std::runtime_error(std::string("Failed to compile vertex shader: ") + message + _vertexSource);
		}

		unsigned int fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentId, 1, &cFragmentSource, NULL);
		glCompileShader(fragmentId);

		glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &isSucceed);
		if (!isSucceed) {
			glGetShaderInfoLog(fragmentId, 512, NULL, message);

			throw std::runtime_error(std::string("Failed to compile fragment shader: ") + message + _fragmentSource);
		}

		terminate(); // in case shader already created

		_id = glCreateProgram();
		glAttachShader(_id, vertexId);
		glAttachShader(_id, fragmentId);
		glLinkProgram(_id);

		glGetProgramiv(_id, GL_LINK_STATUS, &isSucceed);
		if (!isSucceed) {
			glGetProgramInfoLog(_id, 512, NULL, message);

			throw std::runtime_error(std::string("Failed to link shader program: ") + message);
		}

		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);
	}

	void Shader::bind() {
		glUseProgram(_id);
	}

	void Shader::terminate() {
		glDeleteProgram(_id);
	}

	int Shader::getLocation(const std::string& name) {
		return glGetUniformLocation(_id, name.c_str());
	}

	void Shader::setUniformVector2(const std::string& name, const Vector2& vector) {
		glUniform2f(getLocation(name), vector.x, vector.y);
	}

	void Shader::setUniformVector3(const std::string& name, const Vector3& vector) {
		glUniform3f(getLocation(name), vector.x, vector.y, vector.z);
	}

	void Shader::setUniformVector4(const std::string& name, const Vector4& vector) {
		glUniform4f(getLocation(name), vector.x, vector.y, vector.z, vector.w);
	}

	void Shader::setUniformMatrix3(const std::string& name, const Matrix3& matrix) {
		glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, matrix.toPointer());
	}

	void Shader::setUniformMatrix4(const std::string& name, const Matrix4& matrix) {
		glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, matrix.toPointer());
	}

	void Shader::setUniformInt(const std::string& name, int value) {
		glUniform1i(getLocation(name), value);
	}

	void Shader::setUniformInt(const std::string& name, size_t size, int* values) {
		glUniform1iv(getLocation(name), size, values);
	}

	void Shader::setUniformUint(const std::string& name, unsigned int value) {
		glUniform1ui(getLocation(name), value);
	}

	void Shader::setUniformUint(const std::string& name, size_t size, unsigned int* values) {
		glUniform1uiv(getLocation(name), size, values);
	}

	void Shader::setUniformFloat(const std::string& name, float value) {
		glUniform1f(getLocation(name), value);
	}

	void Shader::setUniformFloat(const std::string& name, size_t size, float* values) {
		glUniform1fv(getLocation(name), size, values);
	}
}
