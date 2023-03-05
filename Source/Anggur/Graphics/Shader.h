#pragma once

#include "Anggur/Math/Math.h"
#include <string>

namespace Anggur
{
	class Shader
	{
	public:
		Shader();
		~Shader();

		inline uint32_t GetID() const
		{
			return id;
		}

		void SetVertexSource(const std::string& source);
		void SetFragmentSource(const std::string& source);

		void Compile();
		void Bind();
		void Terminate();

		int GetLocation(const std::string& name);

		void SetUniformMatrix3(const std::string& name, const Matrix3& matrix);
		void SetUniformMatrix4(const std::string& name, const Matrix4& matrix);

		void SetUniformVector2(const std::string& name, const Vector2& vector);
		void SetUniformVector3(const std::string& name, const Vector3& vector);
		void SetUniformVector4(const std::string& name, const Vector4& vector);

		void SetUniformInt(const std::string& name, int value);
		void SetUniformInt(const std::string& name, size_t size, int* values);

		void SetUniformFloat(const std::string& name, float value);
		void SetUniformFloat(const std::string& name, size_t size, float* values);

	private:
		uint32_t id;
		std::string vertexSource;
		std::string fragmentSource;
	};
}
