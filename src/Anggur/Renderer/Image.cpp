#include "Image.h"
#include <Anggur/Helper/IO.h>
#include <Anggur/Helper/Log.h>
#include <stb_image.h>

namespace Anggur
{

	Image::Image()
	{
		_data = nullptr;
	}

	Image::Image(const string& path)
	{
		_data = nullptr;
		Load(path);
	}

	Image::~Image()
	{
		Unload();
	}

	void Image::Load(const string& path)
	{
		Unload();

		int width;
		int height;
		int channels;

		stbi_set_flip_vertically_on_load(1);
		_data = stbi_load(path.c_str(), &width, &height, &channels, 4);
		ANGGUR_ASSERT(_data, "[Renderer.Image] failed to load image\n");

		this->_width = width;
		this->_height = height;
		this->_channels = channels;
	}

	void Image::Unload()
	{
		if (_data)
			stbi_image_free(_data);
	}

} // namespace Anggur
