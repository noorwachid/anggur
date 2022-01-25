#pragma once

#include "Texture.h"

namespace Anggur
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		FrameBuffer(uint width, uint height);
		~FrameBuffer();

		inline uint GetID() const
		{
			return _id;
		}
		inline uint GetColorAttachment() const
		{
			return _colorAttachment;
		}
		inline uint GetWidth() const
		{
			return _width;
		}
		inline uint GetHeight() const
		{
			return _height;
		}

		void Bind();
		void Unbind();
		void Recreate(uint width, uint height);

		Texture ToTexture();

		static void BindDefault();

	private:
		uint _id;
		uint _colorAttachment;
		uint _width;
		uint _height;
	};
}

