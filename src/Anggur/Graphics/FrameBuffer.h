#pragma once

#include "Anggur/Graphics/Texture2D.h"
#include <memory>

namespace Anggur {
	class FrameBuffer {
	public:
		FrameBuffer();
		FrameBuffer(unsigned int width, unsigned int height);

		~FrameBuffer();

		inline unsigned int getID() const {
			return id;
		}

		inline unsigned int getColorAttachment() const {
			return colorAttachment;
		}

		inline unsigned int getWidth() const {
			return width;
		}

		inline unsigned int getHeight() const {
			return height;
		}

		void bind();
		void unbind();

		void recreate(unsigned int width, unsigned int height);

		Texture2D* getTexture() {
			return texture;
		};

	private:
		void Initialize();

	private:
		unsigned int id;
		unsigned int colorAttachment;
		unsigned int width;
		unsigned int height;

		Texture2D* texture = nullptr;
	};
}
