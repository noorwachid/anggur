#include <Anggur/System/File.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Math/Vector2.h>
#include <Anggur/Math/Matrix3.h>
#include <Anggur/Math/Matrix4.h>
#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Shader.h>

namespace Anggur 
{
	Shader::Shader() 
	{
	}

	Shader::~Shader() 
	{
		Terminate();
	}

	void Shader::SetVertexSource(const String& source) 
	{
		vertexSource = source;
	}

	void Shader::SetFragmentSource(const String& source) 
	{
		fragmentSource = source;
	}

	void Shader::Compile() 
	{
		const char* cVertexSource = vertexSource.c_str();
		const char* cFragmentSource = fragmentSource.c_str();
		int isSucceed;
		char message[512];

		uint32_t vertexId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexId, 1, &cVertexSource, NULL);
		glCompileShader(vertexId);

		glGetShaderiv(vertexId, GL_COMPILE_STATUS, &isSucceed);
		if (!isSucceed) 
		{
			glGetShaderInfoLog(vertexId, 512, NULL, message);
			ANGGUR_LOG("[Graphic.Shader.compile] Failed to compile vertex shader");
			ANGGUR_LOG("    %s", message);
		}

		uint32_t fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentId, 1, &cFragmentSource, NULL);
		glCompileShader(fragmentId);

		glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &isSucceed);
		if (!isSucceed) 
		{
			glGetShaderInfoLog(fragmentId, 512, NULL, message);
			ANGGUR_LOG("[Graphic.Shader.compile] failed to compile fragment shader");
			ANGGUR_LOG("    %s", message);
		}

		Terminate(); // in case shader already created

		id = glCreateProgram();
		glAttachShader(id, vertexId);
		glAttachShader(id, fragmentId);
		glLinkProgram(id);

		glGetProgramiv(id, GL_LINK_STATUS, &isSucceed);
		if (!isSucceed) 
		{
			glGetProgramInfoLog(id, 512, NULL, message);
			ANGGUR_LOG("[Graphic.Shader.compile] failed to link shader program");
			ANGGUR_LOG("    %s", message);
		}
		glDeleteShader(vertexId);
		glDeleteShader(fragmentId);
	}

	void Shader::Bind() 
	{
		glUseProgram(id);
	}

	void Shader::Terminate() 
	{
		if (id > 0) {
			glDeleteProgram(id);
		}
	}

	int Shader::GetLocation(const String& name) 
	{
		return glGetUniformLocation(id, name.c_str());
	}

	void Shader::SetUniformVector2(const String& name, const Vector2& vec) 
	{
		glUniform2f(GetLocation(name), vec.x, vec.y);
	}

	void Shader::SetUniformMatrix3(const String& name, const Matrix3& mat) 
	{
		glUniformMatrix3fv(GetLocation(name), 1, GL_FALSE, mat.toPointer());
	}

	void Shader::SetUniformMatrix4(const String& name, const Matrix4& matrix) 
	{
		glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, matrix.toPointer());
	}

	void Shader::SetUniformInt(const String& name, int value) 
	{
		glUniform1i(GetLocation(name), value);
	}

	void Shader::SetUniformInt(const String& name, size_t size, int* values) 
	{
		glUniform1iv(GetLocation(name), size, values);
	}

	void Shader::SetUniformFloat(const String& name, float value) 
	{
		glUniform1f(GetLocation(name), value);
	}

	void Shader::SetUniformFloat(const String& name, size_t size, float* values) 
	{
		glUniform1fv(GetLocation(name), size, values);
	}
}
