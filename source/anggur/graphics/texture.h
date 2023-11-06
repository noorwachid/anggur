#pragma once

#include <vector>

namespace Anggur {
	enum class SamplerFilter {
		nearest,
		linear
	};

	class TextureSpecification {
	public:
		static int getMaxSlot();
	};

	class Texture {
	public:
		Texture(
			const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height, unsigned int channels = 8,
			SamplerFilter filter = SamplerFilter::linear
		);

		~Texture();

		unsigned int getID() const {
			return id;
		}

		unsigned int getWidth() const {
			return width;
		}

		unsigned int getHeight() const {
			return height;
		}

		unsigned int getChannels() const {
			return channels;
		}

		void bind(unsigned int slot = 0);

		friend bool operator==(const Texture& a, const Texture& b);

	private:
		unsigned int id;
		unsigned int width;
		unsigned int height;
		unsigned int channels;
	};
}
