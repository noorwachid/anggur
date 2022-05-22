#pragma once

#include <Anggur/System/String.h>
#include <Anggur/Math/Common.h>

namespace Anggur 
{
	class Shader 
	{
	public:
		Shader();
		~Shader();

		inline uint32_t GetID() const { return id; }

		void SetVertexSource(const String& source);
		void SetFragmentSource(const String& source);

		void Compile();
		void Bind();
		void Terminate();

		int GetLocation(const String& name);

		void SetUniformMatrix3(const String& name, const Matrix3& mat);
		void SetUniformMatrix4(const String& name, const Matrix4& matrix);

		void SetUniformVector2(const String& name, const Vector2& vec);

		void SetUniformInt(const String& name, int value);
		void SetUniformInt(const String& name, size_t size, int* values);

		void SetUniformFloat(const String& name, float value);
		void SetUniformFloat(const String& name, size_t size, float* values);

	private:
		uint32_t id;
		String vertexSource;
		String fragmentSource;
	};
}
