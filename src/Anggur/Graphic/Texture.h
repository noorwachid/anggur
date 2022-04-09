#pragma once
#include "Anggur/Graphic/Image.h"

namespace Anggur {
	enum class SamplerFilter {
		nearest = 0x2600,
		linear = 0x2601
	};

	class Texture {
	public:
		Texture();

		Texture(uint32_t id, uint32_t width, uint32_t height);

		Texture(const std::string& path, SamplerFilter filter = SamplerFilter::linear);

		~Texture();

		inline uint32_t getId() const {
			return id;
		}

		inline uint32_t getWidth() const {
			return width;
		}

		inline uint32_t getHeight() const {
			return height;
		}

		inline uint32_t getChannels() const {
			return channels;
		}

		void loadBitmap(uint8_t* data,
			uint32_t width,
			uint32_t height,
			uint32_t channels = 8,
			SamplerFilter filter = SamplerFilter::linear);

		void load(const Image& image, SamplerFilter filter = SamplerFilter::linear);
		void load(const std::string& path, SamplerFilter filter = SamplerFilter::linear);
		void unload();

		void bind(uint32_t slot = 0);

		friend bool operator==(const Texture& a, const Texture& b);

	private:
		uint32_t id;
		uint32_t width;
		uint32_t height;
		uint32_t channels;
	};
}
