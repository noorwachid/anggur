#pragma once

#include "Anggur/Integral.h"
#include "Anggur/Math/Math.h"
#include <string>

namespace Anggur
{
	class Shader
	{
	public:
		Shader();
		~Shader();

		inline uint GetID() const
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
		void SetUniformInt(const std::string& name, usize size, int* values);

		void SetUniformUint(const std::string& name, uint value);
		void SetUniformUint(const std::string& name, usize size, uint* values);

		void SetUniformFloat(const std::string& name, float value);
		void SetUniformFloat(const std::string& name, usize size, float* values);

	private:
		uint id;
		std::string vertexSource;
		std::string fragmentSource;
	};
}
