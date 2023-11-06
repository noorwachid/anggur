#pragma once

#include <string>
#include <vector>

namespace Anggur {
	class Image {
	public:
		Image();

		Image(const std::string& path);

		~Image();

		inline unsigned int getWidth() const {
			return width;
		}

		inline unsigned int getHeight() const {
			return height;
		}

		inline unsigned int getChannels() const {
			return channels;
		}

		inline unsigned int getVolume() const {
			return width * height * channels;
		}

		inline const std::vector<unsigned char>& getBytes() const {
			return bytes;
		}

		inline std::vector<unsigned char>& getBytes() {
			return bytes;
		}

		inline unsigned char* toPointer() {
			return bytes.data();
		}

		inline void setByte(unsigned int index, unsigned char value) {
			bytes[index] = value;
		}

		inline void setByte(unsigned int x, unsigned int y, unsigned char value) {
			bytes[y * width + x] = value;
		}

		inline void setWidth(unsigned int width) {
			this->width = width;
		}

		inline void setHeight(unsigned int height) {
			this->height = height;
		}

		inline void setSize(unsigned int width, unsigned int height) {
			this->width = width;
			this->height = height;
		}

		inline void setChannels(unsigned int channels) {
			this->channels = channels;
		}

		void reset() {
			bytes.assign(getVolume(), 0);
		}

		void resize() {
			bytes.resize(getVolume());
		}

		void read(const std::string& path);

		void close();

		void write(const std::string& path);

	private:
		unsigned int width;
		unsigned int height;
		unsigned int channels;

		std::vector<unsigned char> bytes;
	};
}
