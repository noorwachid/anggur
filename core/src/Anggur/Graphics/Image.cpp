#include "Image.h"
#include "stb_image.h"
#include <cassert>

namespace Anggur 
{
	Image::Image() 
	{
		data = nullptr;
	}

	Image::Image(const std::string& path) 
	{
		data = nullptr;
		Load(path);
	}

	Image::~Image() 
	{
		Unload();
	}

	void Image::Load(const std::string& path) 
	{
		Unload();

		int width;
		int height;
		int channels;

		data = stbi_load(path.c_str(), &width, &height, &channels, 4);
		assert(data && "[Image] failed to load image");

		this->width = width;
		this->height = height;
		this->channels = channels;
	}

	void Image::Unload() 
	{
		if (data) 
			stbi_image_free(data);
	}
}
