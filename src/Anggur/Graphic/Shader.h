#pragma once

#include <string>
#include <Anggur/Math/Math.h>

namespace Anggur {
	class Shader {
	public:
		uint32_t getId();

		void setVertexSource(const std::string& source);
		void setFragmentSource(const std::string& source);

		void compile();
		void bind();
		void destroy();

		int getLocation(const std::string& name);

		void setMatrix3(const std::string& name, const Matrix3& mat);
		void setVector2(const std::string& name, const Vector2& vec);

		void setInt(const std::string& name, int value);
		void setInt(const std::string& name, size_t size, int* values);

		void setFloat(const std::string& name, float value);
		void setFloat(const std::string& name, size_t size, float* values);

	private:
		uint32_t id;
		std::string vertexSource;
		std::string fragmentSource;
	};
}
