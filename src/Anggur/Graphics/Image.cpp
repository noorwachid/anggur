#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <cassert>

namespace Anggur
{
	Image::Image()
	{
	}

	Image::Image(const std::string& path)
	{
		Read(path);
	}

	Image::~Image()
	{
		Close();
	}

	void Image::Read(const std::string& path)
	{
		Close();

		int newWidth;
		int newHeight;
		int newChannels;

        stbi_set_flip_vertically_on_load(true);

		uint8_t* rawData = stbi_load(path.c_str(), &newWidth, &newHeight, &newChannels, 4);
		assert(rawData && "Failed to load image");

		bytes.resize(newWidth * newHeight * newChannels);
		std::memcpy(bytes.data(), rawData, bytes.size());

		stbi_image_free(rawData);

		width = newWidth;
		height = newHeight;
		channels = newChannels;
	}

	void Image::Close()
	{
	}

	void Image::Write(const std::string& path)
	{
		stbi_write_png((path + ".png").c_str(), width, height, 1, bytes.data(), width);
	}
}
