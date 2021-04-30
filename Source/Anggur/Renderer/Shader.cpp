#include <glad/glad.h>
#include <Anggur/Helper/Io.hpp>
#include <Anggur/Helper/Log.hpp>
#include <Anggur/Math/Vector.hpp>
#include <Anggur/Math/Matrix.hpp>
#include "Shader.hpp"

namespace Anggur {

uint Shader::GetId()
{
    return mId;
}

void Shader::SetVertexSource(const std::string& source)
{
    mVertexSource = source;
}

void Shader::SetFragmentSource(const std::string& source)
{
    mFragmentSource = source;
}

void Shader::Compile()
{
    const char* cVertexSource   = mVertexSource.c_str();
    const char* cFragmentSource = mFragmentSource.c_str();
    int isSucceed;
    char message[512];

    uint vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, &cVertexSource, NULL);
    glCompileShader(vertexId);

    glGetShaderiv(vertexId, GL_COMPILE_STATUS, &isSucceed);
    if (!isSucceed)
    {
        glGetShaderInfoLog(vertexId, 512, NULL, message);
        Anggur_Log("[Renderer.Shader] failed to compile vertex shader\n%s", message);
    }

    uint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &cFragmentSource, NULL);
    glCompileShader(fragmentId);

    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &isSucceed);
    if (!isSucceed)
    {
        glGetShaderInfoLog(fragmentId, 512, NULL, message);
        Anggur_Log("[Renderer.Shader] failed to compile fragment shader\n%s", message);
    }

    mId = glCreateProgram();
    glAttachShader(mId, vertexId);
    glAttachShader(mId, fragmentId);
    glLinkProgram(mId);

    glGetProgramiv(mId, GL_LINK_STATUS, &isSucceed);
    if (!isSucceed) {
        glGetProgramInfoLog(mId, 512, NULL, message);
        Anggur_Log("[Renderer.Shader] failed to link shader program\n%s", message);
    }
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}

void Shader::Bind()
{
    glUseProgram(mId);
}

void Shader::Destroy()
{
    glDeleteProgram(mId);
}


int Shader::GetLocation(const std::string& name)
{
    return glGetUniformLocation(mId, name.c_str());
}

void Shader::SetMatrix(const std::string& name, const Matrix& matrix)
{
    int id = GetLocation(name);
    glUniformMatrix3fv(id, 1, GL_FALSE, matrix.ToFloatPtr());
}

void Shader::SetVector(const std::string& name, const Vector& vec)
{
    int id = GetLocation(name);
    glUniform2f(id, vec.x, vec.y);
}

void Shader::SetInt(const std::string& name, int value)
{
    int id = GetLocation(name);
    glUniform1i(id, value);
}

void Shader::SetInt(const std::string& name, size_t size, int* values)
{
    int id = GetLocation(name);
    glUniform1iv(id, size, values);
}

void Shader::SetFloat(const std::string& name, float value)
{
    int id = GetLocation(name);
    glUniform1f(id, value);
}

void Shader::SetFloat(const std::string& name, size_t size, float* values)
{
    int id = GetLocation(name);
    glUniform1fv(id, size, values);
}

}
