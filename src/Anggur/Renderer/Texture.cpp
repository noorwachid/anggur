#include <glad/glad.h>
#include <stb_image.h>
#include <Anggur/Helper/IO.h>
#include <Anggur/Helper/Log.h>
#include "Texture.h"

namespace Anggur
{

	Texture::Texture()
	{
		_id = 0;
	}

	Texture::Texture(uint32_t id, uint32_t width, uint32_t height)
	{
		this->_id = id;
		this->_width = width;
		this->_height = height;
	}

	Texture::Texture(const string& path, SamplerFilter filter)
	{
		_id = 0;
		Load(path, filter);
	}

	Texture::~Texture()
	{
		Unload();
	}

	void Texture::LoadBitmap(uint8_t* data, uint32_t width, uint32_t height, uint32_t channels, SamplerFilter filter)
	{
		Unload();
		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		this->_width = width;
		this->_height = height;
		this->_channels = channels;

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(filter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(filter));

		if (data)
		{
			int iformat;
			int format;

			switch (channels)
			{
				case 1:
					iformat = GL_R8;
					format = GL_RED;
					break;
				case 4:
					iformat = GL_RGBA8;
					format = GL_RGBA;
					break;
				case 8:
				default:
					iformat = GL_RGB8;
					format = GL_RGB;
					break;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			ANGGUR_LOG("[Texture.LoadBitmap] Data is empty\n");
		}
	}

	void Texture::Load(const Image& image, SamplerFilter filter)
	{
		_width = image.GetWidth();
		_height = image.GetHeight();
		_channels = image.GetChannels();
		LoadBitmap(image.GetData(), _width, _height, _channels, filter);
	}

	void Texture::Load(const string& path, SamplerFilter filter)
	{
		Image image(path);
		Load(image, filter);
	}

	void Texture::Unload()
	{
		if (_id != 0)
			glDeleteTextures(1, &_id);
	}

	void Texture::Bind(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, _id);
	}

	bool operator==(const Texture& a, const Texture& b)
	{
		return a.GetID() == b.GetID();
	}

}
