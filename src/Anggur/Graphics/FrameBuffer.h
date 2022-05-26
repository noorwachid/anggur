#pragma once

#include <Anggur/Graphics/Texture2D.h>
#include <memory>

namespace Anggur {
	class FrameBuffer {
	public:
		FrameBuffer();
		FrameBuffer(uint32_t width, uint32_t height);

		~FrameBuffer();

		inline uint32_t getId() const { return id; }
		inline uint32_t getColorAttachment() const { return colorAttachment; }
		inline uint32_t getWidth() const { return width; }
		inline uint32_t getHeight() const { return height; }

		void bind();
		void unbind();

		void recreate(uint32_t width, uint32_t height);

		const std::shared_ptr<Texture2D>& getTexture() const { return texture; };

	private:
		void Initialize();
		
	private:
		uint32_t id;
		uint32_t colorAttachment;
		uint32_t width;
		uint32_t height;

		std::shared_ptr<Texture2D> texture = nullptr;
	};
}

