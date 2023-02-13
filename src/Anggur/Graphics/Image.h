#pragma once

#include <string>
#include <vector>

namespace Anggur
{
	class Image
	{
	  public:
		Image();
		Image(const std::string& path);

		~Image();

		inline uint32_t GetWidth() const
		{
			return width;
		}

		inline uint32_t GetHeight() const
		{
			return height;
		}

		inline uint32_t GetChannels() const
		{
			return channels;
		}

		inline uint32_t GetVolume() const
		{
			return width * height * channels;
		}

		inline const std::vector<uint8_t>& GetBytes() const
		{
			return bytes;
		}

		inline std::vector<uint8_t>& GetBytes()
		{
			return bytes;
		}

		inline uint8_t* ToPointer()
		{
			return bytes.data();
		}

		inline void SetByte(uint32_t index, uint8_t value)
		{
			bytes[index] = value;
		}

		inline void SetByte(uint32_t x, uint32_t y, uint8_t value)
		{
			bytes[y * width + x] = value;
		}

		inline void SetWidth(uint32_t newWidth)
		{
			width = newWidth;
		}

		inline void SetHeight(uint32_t newHeight)
		{
			height = newHeight;
		}

		inline void SetSize(uint32_t newWidth, uint32_t newHeight)
		{
			width = newWidth;
			height = newHeight;
		}

		inline void SetChannels(uint32_t newChannels)
		{
			channels = newChannels;
		}

		void Resize()
		{
			bytes.resize(GetVolume());
		}

		void Read(const std::string& path);
		void Close();

		void Write(const std::string& path);

	  private:
		uint32_t width;
		uint32_t height;
		uint32_t channels;
		std::vector<uint8_t> bytes;
	};
}
