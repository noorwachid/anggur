#pragma once

#include <string>
#include <Anggur/Math/Math.h>

namespace Anggur {
	class Shader {
	public:
		Shader();
		~Shader();

		uint32_t getId();

		void setVertexSource(const std::string& source);
		void setFragmentSource(const std::string& source);

		void compile();
		void bind();
		void destroy();

		int getLocation(const std::string& name);

		void setUniformMatrix3(const std::string& name, const Matrix3& mat);
		void setUniformMatrix4(const std::string& name, const Matrix4& matrix);

		void setUniformVector2(const std::string& name, const Vector2& vec);

		void setUniformInt(const std::string& name, int value);
		void setUniformInt(const std::string& name, size_t size, int* values);

		void setUniformFloat(const std::string& name, float value);
		void setUniformFloat(const std::string& name, size_t size, float* values);

	private:
		uint32_t id;
		std::string vertexSource;
		std::string fragmentSource;
	};
}
