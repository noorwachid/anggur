#include <stb_image.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Texture.h>

namespace Anggur {

	Texture::Texture() {
		id = 0;
	}

	Texture::Texture(uint32_t id, uint32_t width, uint32_t height) {
		this->id = id;
		this->width = width;
		this->height = height;
	}

	Texture::Texture(const std::string& path, SamplerFilter filter) {
		id = 0;
		load(path, filter);
	}

	Texture::~Texture() {
		unload();
	}

	void Texture::loadBitmap(uint8_t* data, uint32_t width, uint32_t height, uint32_t channels, SamplerFilter filter) {
		unload();
		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
		else {
			ANGGUR_LOG("[Graphic.Texture.loadBitmap] Data is empty");
		}
	}

	void Texture::load(const Image& image, SamplerFilter filter) {
		width = image.getWidth();
		height = image.getHeight();
		channels = image.getChannels();
		loadBitmap(image.getData(), width, height, channels, filter);
	}

	void Texture::load(const std::string& path, SamplerFilter filter) {
		Image image(path);
		load(image, filter);
	}

	void Texture::unload() {
		if (id != 0)
			glDeleteTextures(1, &id);
	}

	void Texture::bind(uint32_t slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	int Texture::getMaxSlot() {
		static int maxSlot = 0;
		if (maxSlot == 0) {
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxSlot);
		}

		return maxSlot;
	}

	bool operator==(const Texture& a, const Texture& b) {
		return a.getId() == b.getId();
	}
}
