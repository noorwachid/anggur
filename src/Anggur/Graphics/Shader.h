#pragma once

#include "Anggur/Math/Math.h"
#include <string>

namespace Anggur {
	class Shader {
	public:
		Shader();

		~Shader();

		inline unsigned int getID() const {
			return id;
		}

		void setVertexSource(const std::string& source);
		void setFragmentSource(const std::string& source);

		void compile();
		void bind();
		void terminate();

		int getLocation(const std::string& name);

		void setUniformMatrix3(const std::string& name, const Matrix3& matrix);
		void setUniformMatrix4(const std::string& name, const Matrix4& matrix);

		void setUniformVector2(const std::string& name, const Vector2& vector);
		void setUniformVector3(const std::string& name, const Vector3& vector);
		void setUniformVector4(const std::string& name, const Vector4& vector);

		void setUniformInt(const std::string& name, int value);
		void setUniformInt(const std::string& name, size_t size, int* values);

		void setUniformUnsignedInt(const std::string& name, unsigned int value);
		void setUniformUnsignedInt(const std::string& name, size_t size, unsigned int* values);

		void setUniformFloat(const std::string& name, float value);
		void setUniformFloat(const std::string& name, size_t size, float* values);

	private:
		unsigned int id;
		std::string vertexSource;
		std::string fragmentSource;
	};
}
