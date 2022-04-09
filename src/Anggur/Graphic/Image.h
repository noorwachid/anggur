#pragma once

#include <string>

namespace Anggur {
	class Image {
	public:
		Image();

		Image(const std::string& path);

		~Image();

		inline uint32_t getWidth() const {
			return width;
		}

		inline uint32_t getHeight() const {
			return height;
		}

		inline uint32_t getChannels() const {
			return channels;
		}

		inline uint8_t* getData() const {
			return data;
		}

		void load(const std::string& path);

		void unload();

	private:
		uint32_t width;
		uint32_t height;
		uint32_t channels;
		uint8_t* data;
	};
}

