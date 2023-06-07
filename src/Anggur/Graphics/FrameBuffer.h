#pragma once

#include "Anggur/Graphics/Texture2D.h"
#include <memory>

namespace Anggur
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		FrameBuffer(unsigned int width, unsigned int height);

		~FrameBuffer();

		inline unsigned int GetID() const
		{
			return id;
		}

		inline unsigned int GetColorAttachment() const
		{
			return colorAttachment;
		}

		inline unsigned int GetWidth() const
		{
			return width;
		}

		inline unsigned int GetHeight() const
		{
			return height;
		}

		void Bind();
		void Unbind();

		void Recreate(unsigned int width, unsigned int height);

		Texture2D* GetTexture()
		{
			return texture;
		};

	private:
		void Initialize();

	private:
		unsigned int id;
		unsigned int colorAttachment;
		unsigned int width;
		unsigned int height;

		Texture2D* texture = nullptr;
	};
}
