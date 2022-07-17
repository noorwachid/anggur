#pragma once

#include <string>

namespace Anggur 
{
	class Image 
	{
	public:
		Image();
		Image(const std::string& path);

		~Image();

		inline uint32_t GetWidth() const { return width; }
		inline uint32_t GetHeight() const { return height; }
		inline uint32_t GetChannels() const { return channels; }
		inline uint8_t* GetData() const { return data; }

		void Load(const std::string& path);
		void Unload();

	private:
		uint32_t width;
		uint32_t height;
		uint32_t channels;
		uint8_t* data;
	};
}

