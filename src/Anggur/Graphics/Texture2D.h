#pragma once

#include "Anggur/Graphics/TextureSpecification.h"
#include <vector>

namespace Anggur {
	class Texture2D {
	public:
		Texture2D();
		Texture2D(
			const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height, unsigned int channels = 8,
			SamplerFilter filter = SamplerFilter::linear
		);

		~Texture2D();

		inline unsigned int getID() const {
			return id;
		}

		inline unsigned int getWidth() const {
			return width;
		}

		inline unsigned int getHeight() const {
			return height;
		}

		inline unsigned int getChannels() const {
			return channels;
		}

		void read(
			const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height, unsigned int channels = 8,
			SamplerFilter filter = SamplerFilter::linear
		);

		void close();

		void bind(unsigned int slot = 0);

		friend bool operator==(const Texture2D& a, const Texture2D& b);
		friend class FrameBuffer;

	private:
		unsigned int id;
		unsigned int width;
		unsigned int height;
		unsigned int channels;
	};
}
