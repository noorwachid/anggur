#pragma once
#include <Anggur/Graphics/Image.h>

namespace Anggur 
{
	enum class SamplerFilter 
	{
		Nearest = 0x2600,
		Linear = 0x2601
	};

	class Texture2D
	{
	public:
		Texture2D();
		Texture2D(uint32_t id, uint32_t width, uint32_t height);
		Texture2D(const std::string& path, SamplerFilter filter = SamplerFilter::Linear);
		Texture2D(uint8_t* data, uint32_t width, uint32_t height, uint32_t channels = 8, SamplerFilter filter = SamplerFilter::Linear);

		~Texture2D();

		inline uint32_t GetID() const { return id; }
		inline uint32_t GetWidth() const { return width; }
		inline uint32_t GetHeight() const { return height; }
		inline uint32_t GetChannels() const { return channels; }

		void Load(uint8_t* data, uint32_t width, uint32_t height, uint32_t channels = 8, SamplerFilter filter = SamplerFilter::Linear);
		void Load(const Image& image, SamplerFilter filter = SamplerFilter::Linear);
		void Load(const std::string& path, SamplerFilter filter = SamplerFilter::Linear);
		void Unload();

		void Bind(uint32_t slot = 0);

		static int GetMaxSlot();

		friend bool operator==(const Texture2D& a, const Texture2D& b);

	private:
		uint32_t id;
		uint32_t width;
		uint32_t height;
		uint32_t channels;

		static int maxSlot;
	};
}
