#include "Anggur/Graphics/Texture.h"
#include "glad/glad.h"
#include "stb_image.h"
#include <vector>

namespace Anggur
{
	int TextureSpecification::GetMaxSlot()
	{
		static int maxSlot = 0;
		if (maxSlot == 0)
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxSlot);

		return maxSlot;
	}

	Texture::Texture()
	{
		_id = 0;
	}

	Texture::Texture(
		const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height, unsigned int channels,
		SamplerFilter filter
	)
	{
		_id = 0;
		Read(bytes, width, height, channels, filter);
	}

	Texture::~Texture()
	{
		Close();
	}

	int toGLFilter(SamplerFilter filter)
	{
		switch (filter)
		{
			case SamplerFilter::Linear:
				return GL_LINEAR;
			case SamplerFilter::Nearest:
				return GL_NEAREST;
			default:
				return 0;
		}
	}

	void Texture::Read(
		const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height, unsigned int channels,
		SamplerFilter filter
	)
	{
		Close();
		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		this->_width = width;
		this->_height = height;
		this->_channels = channels;

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilter(filter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLFilter(filter));

		if (bytes.empty())
		{
			assert(false && "Cannot load buffer to texture, data is empty");
		}
		else
		{
			int iformat;
			int format;

			switch (channels)
			{
				case 1:
					iformat = GL_R8;
					format = GL_RED;
					break;

				case 3:
					iformat = GL_RGB8;
					format = GL_RGB;
					break;

				case 4:
				default:
					iformat = GL_RGBA8;
					format = GL_RGBA;
					break;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, format, GL_UNSIGNED_BYTE, bytes.data());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	void Texture::Close()
	{
		if (_id != 0)
			glDeleteTextures(1, &_id);
	}

	void Texture::Bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, _id);
	}

	bool operator==(const Texture& a, const Texture& b)
	{
		return a.GetID() == b.GetID();
	}
}
