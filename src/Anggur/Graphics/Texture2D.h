#pragma once

#include "Anggur/Integral.h"
#include "Anggur/Graphics/TextureSpecification.h"
#include <vector>

namespace Anggur
{
	class Texture2D
	{
	public:
		Texture2D();
		Texture2D(
			const std::vector<uchar>& bytes, uint width, uint height, uint channels = 8,
			SamplerFilter filter = SamplerFilter::Linear
		);

		~Texture2D();

		inline uint GetID() const
		{
			return id;
		}

		inline uint GetWidth() const
		{
			return width;
		}

		inline uint GetHeight() const
		{
			return height;
		}

		inline uint GetChannels() const
		{
			return channels;
		}

		void Read(
			const std::vector<uchar>& bytes, uint width, uint height, uint channels = 8,
			SamplerFilter filter = SamplerFilter::Linear
		);

		void Close();

		void Bind(uint slot = 0);

		friend bool operator==(const Texture2D& a, const Texture2D& b);
		friend class FrameBuffer;

	private:
		uint id;
		uint width;
		uint height;
		uint channels;
	};
}
