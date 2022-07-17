#include "Image.h"
#include "stb_image.h"
#include <cassert>

namespace Anggur {
	Image::Image() {
		data = nullptr;
	}

	Image::Image(const std::string& path) {
		data = nullptr;
		load(path);
	}

	Image::~Image() {
		unload();
	}

	void Image::load(const std::string& path) {
		unload();

		int width;
		int height;
		int channels;

		stbi_set_flip_vertically_on_load(1);
		data = stbi_load(path.c_str(), &width, &height, &channels, 4);
		assert(data && "[Image] failed to load image");

		this->width = width;
		this->height = height;
		this->channels = channels;
	}

	void Image::unload() {
		if (data) 
			stbi_image_free(data);
	}
}
