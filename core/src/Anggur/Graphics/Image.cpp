#include <stb_image.h>
#include <Anggur/Utility/Assert.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Graphics/Image.h>

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

		stbi_set_flip_vertically_on_load(1);
		data = stbi_load(path.c_str(), &width, &height, &channels, 4);
		ANGGUR_ASSERT(data, "[Graphics.Image.load] failed to load image: %s", path.c_str());

		this->width = width;
		this->height = height;
		this->channels = channels;
	}

	void Image::Unload() 
	{
		if (data) 
		{
			stbi_image_free(data);
		}
	}
}