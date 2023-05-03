#pragma once

#include "Anggur/Graphics/Texture2D.h"
#include <memory>

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
			return id;
		}

		inline uint GetColorAttachment() const
		{
			return colorAttachment;
		}

		inline uint GetWidth() const
		{
			return width;
		}

		inline uint GetHeight() const
		{
			return height;
		}

		void Bind();
		void Unbind();

		void Recreate(uint width, uint height);

		Texture2D* GetTexture()
		{
			return texture;
		};

	private:
		void Initialize();

	private:
		uint id;
		uint colorAttachment;
		uint width;
		uint height;

		Texture2D* texture = nullptr;
	};
}
