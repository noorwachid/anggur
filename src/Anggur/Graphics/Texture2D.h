#pragma once

#include "Anggur/Graphics/TextureSpecification.h"
#include <vector>

namespace Anggur
{
	class Texture2D
	{
	public:
		Texture2D();
		Texture2D(
			const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height, unsigned int channels = 8,
			SamplerFilter filter = SamplerFilter::Linear
		);

		~Texture2D();

		inline unsigned int GetID() const
		{
			return id;
		}

		inline unsigned int GetWidth() const
		{
			return width;
		}

		inline unsigned int GetHeight() const
		{
			return height;
		}

		inline unsigned int GetChannels() const
		{
			return channels;
		}

		void Read(
			const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height, unsigned int channels = 8,
			SamplerFilter filter = SamplerFilter::Linear
		);

		void Close();

		void Bind(unsigned int slot = 0);

		friend bool operator==(const Texture2D& a, const Texture2D& b);
		friend class FrameBuffer;

	private:
		unsigned int id;
		unsigned int width;
		unsigned int height;
		unsigned int channels;
	};
}
