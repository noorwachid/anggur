#include "Shader.h"
#include "Anggur/Math/Matrix3.h"
#include "Anggur/Math/Matrix4.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector3.h"
#include "Anggur/Math/Vector4.h"
#include "Function.h"
#include <cassert>
#include <stdexcept>

namespace Anggur
{
    Shader::Shader()
    {
    }

    Shader::~Shader()
    {
        terminate();
    }

    void Shader::setVertexSource(const std::string &source)
    {
        vertexSource = source;
    }

    void Shader::setFragmentSource(const std::string &source)
    {
        fragmentSource = source;
    }

    void Shader::compile()
    {
        const char *cVertexSource = vertexSource.c_str();
        const char *cFragmentSource = fragmentSource.c_str();
        int isSucceed;
        char message[512];

        uint32_t vertexId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexId, 1, &cVertexSource, NULL);
        glCompileShader(vertexId);

        glGetShaderiv(vertexId, GL_COMPILE_STATUS, &isSucceed);
        if (!isSucceed)
        {
            glGetShaderInfoLog(vertexId, 512, NULL, message);

            throw std::runtime_error(std::string("Failed to compile vertex shader: ") + message);
        }

        uint32_t fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentId, 1, &cFragmentSource, NULL);
        glCompileShader(fragmentId);

        glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &isSucceed);
        if (!isSucceed)
        {
            glGetShaderInfoLog(fragmentId, 512, NULL, message);

            throw std::runtime_error(std::string("Failed to compile fragment shader: ") + message);
        }

        terminate(); // in case shader already created

        id = glCreateProgram();
        glAttachShader(id, vertexId);
        glAttachShader(id, fragmentId);
        glLinkProgram(id);

        glGetProgramiv(id, GL_LINK_STATUS, &isSucceed);
        if (!isSucceed)
        {
            glGetProgramInfoLog(id, 512, NULL, message);

            throw std::runtime_error(std::string("Failed to link shader program: ") + message);
        }

        glDeleteShader(vertexId);
        glDeleteShader(fragmentId);
    }

    void Shader::bind()
    {
        glUseProgram(id);
    }

    void Shader::terminate()
    {
        if (id > 0)
            glDeleteProgram(id);
    }

    int Shader::getLocation(const std::string &name)
    {
        return glGetUniformLocation(id, name.c_str());
    }

    void Shader::setUniformVector2(const std::string &name, const Vector2 &vector)
    {
        glUniform2f(getLocation(name), vector.x, vector.y);
    }

    void Shader::setUniformVector3(const std::string &name, const Vector3 &vector)
    {
        glUniform3f(getLocation(name), vector.x, vector.y, vector.z);
    }

    void Shader::setUniformVector4(const std::string &name, const Vector4 &vector)
    {
        glUniform4f(getLocation(name), vector.x, vector.y, vector.z, vector.w);
    }

    void Shader::setUniformMatrix3(const std::string &name, const Matrix3 &matrix)
    {
        glUniformMatrix3fv(getLocation(name), 1, GL_FALSE, matrix.toPointer());
    }

    void Shader::setUniformMatrix4(const std::string &name, const Matrix4 &matrix)
    {
        glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, matrix.toPointer());
    }

    void Shader::setUniformInt(const std::string &name, int value)
    {
        glUniform1i(getLocation(name), value);
    }

    void Shader::setUniformInt(const std::string &name, size_t size, int *values)
    {
        glUniform1iv(getLocation(name), size, values);
    }

    void Shader::setUniformFloat(const std::string &name, float value)
    {
        glUniform1f(getLocation(name), value);
    }

    void Shader::setUniformFloat(const std::string &name, size_t size, float *values)
    {
        glUniform1fv(getLocation(name), size, values);
    }
}
