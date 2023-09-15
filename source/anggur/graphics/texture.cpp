#include "anggur/graphics/texture.h"
#include "anggur/graphics/api.h"
#include "stb_image.h"
#include <vector>

namespace Anggur {
	int ToNativeType(SamplerFilter filter) {
		switch (filter) {
			case SamplerFilter::linear:
				return GL_LINEAR;
			case SamplerFilter::nearest:
				return GL_NEAREST;
			default:
				return 0;
		}
	}

	int TextureSpecification::getMaxSlot() {
		static int maxSlot = 0;
		if (maxSlot == 0)
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxSlot);

		return maxSlot;
	}

	Texture::Texture(
		const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height, unsigned int channels,
		SamplerFilter filter
	) {
		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		_width = width;
		_height = height;
		_channels = channels;

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToNativeType(filter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToNativeType(filter));

		if (bytes.empty()) {
			// assert(false && "Cannot load buffer to texture, data is empty");
		} else {
			int iformat;
			int format;

			switch (channels) {
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

	Texture::~Texture() {
		glDeleteTextures(1, &_id);
	}

	void Texture::bind(unsigned int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, _id);
	}

	bool operator==(const Texture& a, const Texture& b) {
		return a.getID() == b.getID();
	}
}
