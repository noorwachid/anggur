#pragma once
#include <Anggur/Graphics/Texture.h>

namespace Anggur {
	class Texture2D {
	public:
		Texture2D();
		Texture2D(const std::string& path, SamplerFilter filter = SamplerFilter::linear);
		Texture2D(uint8_t* data, uint32_t width, uint32_t height, uint32_t channels = 8, SamplerFilter filter = SamplerFilter::linear);

		~Texture2D();

		inline uint32_t getId() const { return id; }
		inline uint32_t getWidth() const { return width; }
		inline uint32_t getHeight() const { return height; }
		inline uint32_t getChannels() const { return channels; }

		void load(uint8_t* data, uint32_t width, uint32_t height, uint32_t channels = 8, SamplerFilter filter = SamplerFilter::linear);
		void load(const Image& image, SamplerFilter filter = SamplerFilter::linear);
		void load(const std::string& path, SamplerFilter filter = SamplerFilter::linear);
		void unload();

		void bind(uint32_t slot = 0);

		friend bool operator==(const Texture2D& a, const Texture2D& b);
		friend class FrameBuffer;

	private:
		uint32_t id;
		uint32_t width;
		uint32_t height;
		uint32_t channels;
	};
}
