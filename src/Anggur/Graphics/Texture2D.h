#pragma once
#include <Anggur/Graphics/Texture.h>

namespace Anggur 
{
	class Texture2D
	{
	public:
		Texture2D();
		Texture2D(const std::string& path, SamplerFilter filter = SamplerFilter::Linear);
		Texture2D(uint8_t* data, uint32_t width, uint32_t height, uint32_t channels = 8, SamplerFilter filter = SamplerFilter::Linear);

		~Texture2D();

		inline uint32_t GetID() const { return id; }
		inline uint32_t GetWidth() const { return width; }
		inline uint32_t GetHeight() const { return height; }
		inline uint32_t GetChannels() const { return channels; }

		void Load(uint8_t* data, uint32_t width, uint32_t height, uint32_t channels = 8, SamplerFilter filter = SamplerFilter::Linear);
		void Load(const Image& image, SamplerFilter filter = SamplerFilter::Linear);
		void Load(const std::string& path, SamplerFilter filter = SamplerFilter::Linear);
		void Unload();

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
