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
			return _width;
		}

		inline unsigned int getHeight() const {
			return _height;
		}

		inline unsigned int getChannels() const {
			return _channels;
		}

		inline unsigned int getVolume() const {
			return _width * _height * _channels;
		}

		inline const std::vector<unsigned char>& getBytes() const {
			return _bytes;
		}

		inline std::vector<unsigned char>& getBytes() {
			return _bytes;
		}

		inline unsigned char* toPointer() {
			return _bytes.data();
		}

		inline void setByte(unsigned int index, unsigned char value) {
			_bytes[index] = value;
		}

		inline void setByte(unsigned int x, unsigned int y, unsigned char value) {
			_bytes[y * _width + x] = value;
		}

		inline void setWidth(unsigned int newWidth) {
			_width = newWidth;
		}

		inline void setHeight(unsigned int newHeight) {
			_height = newHeight;
		}

		inline void setSize(unsigned int newWidth, unsigned int newHeight) {
			_width = newWidth;
			_height = newHeight;
		}

		inline void setChannels(unsigned int newChannels) {
			_channels = newChannels;
		}

		void reset() {
			_bytes.assign(getVolume(), 0);
		}

		void resize() {
			_bytes.resize(getVolume());
		}

		void read(const std::string& path);

		void close();

		void write(const std::string& path);

	private:
		unsigned int _width;
		unsigned int _height;
		unsigned int _channels;
		std::vector<unsigned char> _bytes;
	};
}
