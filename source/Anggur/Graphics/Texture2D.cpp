#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Graphics/Function.h"
#include <stb_image.h>
#include <vector>

namespace Anggur {
	Texture2D::Texture2D() {
		id = 0;
	}

	Texture2D::Texture2D(
		const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height, unsigned int channels,
		SamplerFilter filter
	) {
		id = 0;
		read(bytes, width, height, channels, filter);
	}

	Texture2D::~Texture2D() {
		close();
	}

	int toGLFilter(SamplerFilter filter) {
		switch (filter) {
			case SamplerFilter::linear: return GL_LINEAR;
			case SamplerFilter::nearest: return GL_NEAREST;
			default: return 0;
		}
	}

	void Texture2D::read(
		const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height, unsigned int channels,
		SamplerFilter filter
	) {
		close();
		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		this->width = width;
		this->height = height;
		this->channels = channels;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilter(filter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLFilter(filter));

		if (bytes.empty()) {
			assert(false && "Cannot load buffer to texture, data is empty");
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

	void Texture2D::close() {
		if (id != 0)
			glDeleteTextures(1, &id);
	}

	void Texture2D::bind(unsigned int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	bool operator==(const Texture2D& a, const Texture2D& b) {
		return a.getID() == b.getID();
	}
}
