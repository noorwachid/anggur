#include <Anggur/Utility/File.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Math/Vector2.h>
#include <Anggur/Math/Matrix3.h>
#include <Anggur/Graphic/Function.h>
#include <Anggur/Graphic/Shader.h>

namespace Anggur {

	uint32_t Shader::getId() {
		return id;
	}

	void Shader::setVertexSource(const std::string& source) {
		vertexSource = source;
	}

	void Shader::setFragmentSource(const std::string& source) {
		fragmentSource = source;
	}

	void Shader::compile() {
		const char* cVertexSource = vertexSource.c_str();
		const char* cFragmentSource = fragmentSource.c_str();
		int isSucceed;
		char message[512];

		uint32_t vertexId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexId, 1, &cVertexSource, NULL);
		glCompileShader(vertexId);

		glGetShaderiv(vertexId, GL_COMPILE_STATUS, &isSucceed);
		if (!isSucceed) {
			glGetShaderInfoLog(vertexId, 512, NULL, message);
			ANGGUR_LOG("[Graphic.Shader.compile] Failed to compile vertex shader");
			ANGGUR_LOG("    %s", message);
		}

		uint32_t fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentId, 1, &cFragmentSource, NULL);
		glCompileShader(fragmentId);

		glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &isSucceed);
		if (!isSucceed) {
			glGetShaderInfoLog(fragmentId, 512, NULL, message);
			ANGGUR_LOG("[Graphic.Shader.compile] failed to compile fragment shader");
			ANGGUR_LOG("    %s", message);
		}

		id = glCreateProgram();
		glAttachShader(id, vertexId);
		glAttachShader(id, fragmentId);
		glLinkProgram(id);

		glGetProgramiv(id, GL_LINK_STATUS, &isSucceed);
		if (!isSucceed) {
			glGetProgramInfoLog(id, 512, NULL, message);
			ANGGUR_LOG("[Graphic.Shader.compile] failed to link shader program");
			ANGGUR_LOG("    %s", message);
		}
		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);
	}

	void Shader::bind() {
		glUseProgram(id);
	}

	void Shader::destroy() {
		glDeleteProgram(id);
	}

	int Shader::getLocation(const std::string& name) {
		return glGetUniformLocation(id, name.c_str());
	}

	void Shader::setMatrix3(const std::string& name, const Matrix3& mat) {
		int locId = getLocation(name);
		glUniformMatrix3fv(locId, 1, GL_FALSE, mat.toPointer());
	}

	void Shader::setVector2(const std::string& name, const Vector2& vec) {
		int locId = getLocation(name);
		glUniform2f(locId, vec.x, vec.y);
	}

	void Shader::setInt(const std::string& name, int value) {
		int locId = getLocation(name);
		glUniform1i(locId, value);
	}

	void Shader::setInt(const std::string& name, size_t size, int* values) {
		int locId = getLocation(name);
		glUniform1iv(locId, size, values);
	}

	void Shader::setFloat(const std::string& name, float value) {
		int locId = getLocation(name);
		glUniform1f(locId, value);
	}

	void Shader::setFloat(const std::string& name, size_t size, float* values) {
		int locId = getLocation(name);
		glUniform1fv(locId, size, values);
	}
}
