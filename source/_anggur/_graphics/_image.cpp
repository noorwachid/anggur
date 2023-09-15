#include "Anggur/Graphics/Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <cassert>

namespace Anggur {
	Image::Image() {
	}

	Image::Image(const std::string& path) {
		read(path);
	}

	Image::~Image() {
		close();
	}

	void Image::read(const std::string& path) {
		close();

		int newWidth;
		int newHeight;
		int newChannels;

		// stbi_set_flip_vertically_on_load(true);

		unsigned char* rawData = stbi_load(path.c_str(), &newWidth, &newHeight, &newChannels, 4);
		assert(rawData && "Failed to load image");

		_bytes.resize(newWidth * newHeight * newChannels);
		std::memcpy(_bytes.data(), rawData, _bytes.size());

		stbi_image_free(rawData);

		_width = newWidth;
		_height = newHeight;
		_channels = newChannels;
	}

	void Image::close() {
	}

	void Image::write(const std::string& path) {
		stbi_write_png((path + ".png").c_str(), _width, _height, 1, _bytes.data(), _width);
	}
}
