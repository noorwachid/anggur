#pragma once
#include <string>
#include <Anggur/Math/Math.h>

namespace Anggur
{
	using std::string;

	class Shader
	{
	public:
		uint GetID();
		void SetVertexSource(const string& source);
		void SetFragmentSource(const string& source);
		void Compile();
		void Bind();
		void Destroy();

		int GetLocation(const string& name);

		void SetMatrix(const string& name, const Matrix& mat);
		void SetVector(const string& name, const Vector& vec);
		void SetInt(const string& name, int value);
		void SetInt(const string& name, size_t size, int* values);
		void SetFloat(const string& name, float value);
		void SetFloat(const string& name, size_t size, float* values);

	private:
		uint _id;
		string _vertexSource;
		string _fragmentSource;
	};
}
