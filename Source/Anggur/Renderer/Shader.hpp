#pragma once
#include <string>
#include <Anggur/Math/Math.hpp>

namespace Anggur {

class Shader
{
public:
    uint GetID();
    void SetVertexSource(const std::string& source);
    void SetFragmentSource(const std::string& source);
    void Compile();
    void Bind();
    void Destroy();

    int GetLocation(const std::string& name);

    void SetMatrix(const std::string& name, const Matrix& mat);
    void SetVector(const std::string& name, const Vector& vec);
    void SetInt(const std::string& name, int value);
    void SetInt(const std::string& name, usize size, int* values);
    void SetFloat(const std::string& name, float value);
    void SetFloat(const std::string& name, usize size, float* values);

private:
    uint id;
    std::string vertexSource;
    std::string fragmentSource;

};

}
