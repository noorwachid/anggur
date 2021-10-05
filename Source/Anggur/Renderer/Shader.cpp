#include <glad/glad.h>
#include <Anggur/Helper/IO.h>
#include <Anggur/Helper/Log.h>
#include <Anggur/Math/Vector.h>
#include <Anggur/Math/Matrix.h>
#include "Shader.h"

namespace Anggur {

uint Shader::GetID()
{
    return id;
}

void Shader::SetVertexSource(const string& source)
{
    vertexSource = source;
}

void Shader::SetFragmentSource(const string& source)
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
        ANGGUR_LOG("[Renderer.Shader] failed to compile vertex shader\n%s", message);
    }

    uint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &cFragmentSource, NULL);
    glCompileShader(fragmentId);

    glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &isSucceed);
    if (!isSucceed)
    {
        glGetShaderInfoLog(fragmentId, 512, NULL, message);
        ANGGUR_LOG("[Renderer.Shader] failed to compile fragment shader\n%s", message);
    }

    id = glCreateProgram();
    glAttachShader(id, vertexId);
    glAttachShader(id, fragmentId);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &isSucceed);
    if (!isSucceed) {
        glGetProgramInfoLog(id, 512, NULL, message);
        ANGGUR_LOG("[Renderer.Shader] failed to link shader program\n%s", message);
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


int Shader::GetLocation(const string& name)
{
    return glGetUniformLocation(id, name.c_str());
}

void Shader::SetMatrix(const string& name, const Matrix& matrix)
{
    int locId = GetLocation(name);
    glUniformMatrix3fv(locId, 1, GL_FALSE, matrix.ToFloatPtr());
}

void Shader::SetVector(const string& name, const Vector& vec)
{
    int locId = GetLocation(name);
    glUniform2f(locId, vec.x, vec.y);
}

void Shader::SetInt(const string& name, int value)
{
    int locId = GetLocation(name);
    glUniform1i(locId, value);
}

void Shader::SetInt(const string& name, size_t size, int* values)
{
    int locId = GetLocation(name);
    glUniform1iv(locId, size, values);
}

void Shader::SetFloat(const string& name, float value)
{
    int locId = GetLocation(name);
    glUniform1f(locId, value);
}

void Shader::SetFloat(const string& name, size_t size, float* values)
{
    int locId = GetLocation(name);
    glUniform1fv(locId, size, values);
}

}
