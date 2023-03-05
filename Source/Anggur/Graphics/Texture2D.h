#pragma once

#include "Anggur/Graphics/Image.h"
#include "Anggur/Graphics/TextureSpecification.h"

namespace Anggur
{
	class Texture2D
	{
	public:
		Texture2D();
		Texture2D(
			const std::vector<uint8_t>& bytes, uint32_t width, uint32_t height, uint32_t channels = 8,
			SamplerFilter filter = SamplerFilter::Linear
		);
		Texture2D(const Image& image, SamplerFilter filter = SamplerFilter::Linear);

		~Texture2D();

		inline uint32_t GetID() const
		{
			return id;
		}

		inline uint32_t GetWidth() const
		{
			return width;
		}

		inline uint32_t GetHeight() const
		{
			return height;
		}

		inline uint32_t GetChannels() const
		{
			return channels;
		}

		void Read(
			const std::vector<uint8_t>& bytes, uint32_t width, uint32_t height, uint32_t channels = 8,
			SamplerFilter filter = SamplerFilter::Linear
		);
		void Read(const Image& image, SamplerFilter filter = SamplerFilter::Linear);
		void Close();

		void Bind(uint32_t slot = 0);

		friend bool operator==(const Texture2D& a, const Texture2D& b);
		friend class FrameBuffer;

	private:
		uint32_t id;
		uint32_t width;
		uint32_t height;
		uint32_t channels;
	};
}
