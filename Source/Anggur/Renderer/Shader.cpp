#include <glad/glad.h>
#include <Anggur/Helper/Io.hpp>
#include <Anggur/Helper/Log.hpp>
#include <Anggur/Math/Vector.hpp>
#include <Anggur/Math/Matrix.hpp>
#include "Shader.hpp"

namespace Anggur {

uint Shader::GetId()
{
    return id;
}

void Shader::SetVertexSource(const std::string& source)
{
    vertexSource = source;
}

void Shader::SetFragmentSource(const std::string& source)
{
    fragmentSource = source;
}

void Shader::Compile()
{
    const char* cVertexSource   = vertexSource.c_str();
    const char* cFragmentSource = fragmentSource.c_str();
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

    id = glCreateProgram();
    glAttachShader(id, vertexId);
    glAttachShader(id, fragmentId);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &isSucceed);
    if (!isSucceed) {
        glGetProgramInfoLog(id, 512, NULL, message);
        Anggur_Log("[Renderer.Shader] failed to link shader program\n%s", message);
    }
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}

void Shader::Bind()
{
    glUseProgram(id);
}

void Shader::Destroy()
{
    glDeleteProgram(id);
}


int Shader::GetLocation(const std::string& name)
{
    return glGetUniformLocation(id, name.c_str());
}

void Shader::SetMatrix(const std::string& name, const Matrix& matrix)
{
    int locId = GetLocation(name);
    glUniformMatrix3fv(locId, 1, GL_FALSE, matrix.ToFloatPtr());
}

void Shader::SetVector(const std::string& name, const Vector& vec)
{
    int locId = GetLocation(name);
    glUniform2f(locId, vec.x, vec.y);
}

void Shader::SetInt(const std::string& name, int value)
{
    int locId = GetLocation(name);
    glUniform1i(locId, value);
}

void Shader::SetInt(const std::string& name, usize size, int* values)
{
    int locId = GetLocation(name);
    glUniform1iv(locId, size, values);
}

void Shader::SetFloat(const std::string& name, float value)
{
    int locId = GetLocation(name);
    glUniform1f(locId, value);
}

void Shader::SetFloat(const std::string& name, usize size, float* values)
{
    int locId = GetLocation(name);
    glUniform1fv(locId, size, values);
}

}
