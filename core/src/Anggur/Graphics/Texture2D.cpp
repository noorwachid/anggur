#include <stb_image.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Texture2D.h>
#include <vector>

namespace Anggur {
	Texture2D::Texture2D() {
		id = 0;
	}

	Texture2D::Texture2D(const std::string& path, SamplerFilter filter) {
		id = 0;
		load(path, filter);
	}

	Texture2D::Texture2D(uint8_t* data, uint32_t width, uint32_t height, uint32_t channels, SamplerFilter filter) {
		load(data, width, height, channels, filter);
	}

	Texture2D::~Texture2D() {
		unload();
	}

	void Texture2D::load(uint8_t* data, uint32_t width, uint32_t height, uint32_t channels, SamplerFilter filter) {
		unload();
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(filter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(filter));

		if (data) {
			int iformat;
			int format;

			switch (channels) {
				case 1:
					iformat = GL_R8;
					format = GL_RED;
					break;

				case 3:
				case 4:
				default:
					iformat = GL_RGBA8;
					format = GL_RGBA;
					break;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			ANGGUR_LOG("[Graphic.Texture.loadBitmap] Data is empty");
		}
	}

	void Texture2D::load(const Image& image, SamplerFilter filter) {
		width = image.getWidth();
		height = image.getHeight();
		channels = image.getChannels();
		load(image.getData(), width, height, channels, filter);
	}

	void Texture2D::load(const std::string& path, SamplerFilter filter) {
		Image image(path);
		load(image, filter);
	}

	void Texture2D::unload() {
		if (id != 0)
			glDeleteTextures(1, &id);
	}

	void Texture2D::bind(uint32_t slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	bool operator==(const Texture2D& a, const Texture2D& b) {
		return a.getId() == b.getId();
	}
}
