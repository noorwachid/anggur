#pragma once

#include "Anggur/Integral.h"

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
		void setData(usize byteSize, void* byteData);

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
		void setData(usize byteSize, void* byteData);

	private:
		uint id;
	};

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		uint GetID()
		{
			return id;
		}

		void Bind();
		void Unbind();
		void SetAttribute(usize index, usize size, int type, usize byteStride, usize byteOffset);

	private:
		uint id;
	};
}
