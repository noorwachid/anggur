#pragma once

#include "Anggur/Integral.h"
#include <vector>

namespace Anggur
{
	class IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		uint GetID()
		{
			return id;
		}

		void Bind();
		void Unbind();
		void SetCapacity(usize byteSize);
		void SetData(usize byteSize, void* byteData);

	private:
		uint id = 0;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		uint GetID()
		{
			return id;
		}

		void Bind();
		void Unbind();
		void SetCapacity(usize byteSize);
		void SetData(usize byteSize, void* byteData);

	private:
		uint id;
	};
	
	enum class VertexDataType 
	{
		Float,
	};

	usize GetVertexDataTypeByteSize(VertexDataType type);

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		uint GetID()
		{
			return id;
		}

		usize GetStride() { return stride; }

		void Bind();
		void Unbind();
		void SetAttribute(usize index, usize size, VertexDataType type, usize byteStride, usize byteOffset);
		void SetLayout(const std::vector<std::tuple<usize, VertexDataType>>& layout);

	private:
		uint id = 0;
		usize stride = 0;
	};
}
