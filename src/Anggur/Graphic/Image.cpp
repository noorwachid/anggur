#include "Anggur/Core/IO.h"
#include "Anggur/Core/Log.h"
#include "Anggur/Graphic/Image.h"
#include <stb_image.h>

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
		ANGGUR_ASSERT(data, "[Graphic.Image] failed to load image\n");

		this->width = width;
		this->height = height;
		this->channels = channels;
	}

	void Image::unload() {
		if (data)
			stbi_image_free(data);
	}

} // namespace Anggur
