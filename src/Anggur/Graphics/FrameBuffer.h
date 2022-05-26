#pragma once

#include <Anggur/Graphics/Texture2D.h>
#include <memory>

namespace Anggur 
{
	class FrameBuffer 
	{
	public:
		FrameBuffer();
		FrameBuffer(uint32_t width, uint32_t height);

		~FrameBuffer();

		inline uint32_t GetID() const { return id; }
		inline uint32_t GetColorAttachment() const { return colorAttachment; }
		inline uint32_t GetWidth() const { return width; }
		inline uint32_t GetHeight() const { return height; }

		void Bind();
		void Unbind();

		void Recreate(uint32_t width, uint32_t height);

		const std::shared_ptr<Texture2D>& GetTexture() const { return texture; };

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

