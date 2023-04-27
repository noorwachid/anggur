#pragma once

#include "Anggur/Integral.h"
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

		inline uint GetWidth() const
		{
			return width;
		}

		inline uint GetHeight() const
		{
			return height;
		}

		inline uint GetChannels() const
		{
			return channels;
		}

		inline uint GetVolume() const
		{
			return width * height * channels;
		}

		inline const std::vector<uchar>& GetBytes() const
		{
			return bytes;
		}

		inline std::vector<uchar>& GetBytes()
		{
			return bytes;
		}

		inline uchar* ToPointer()
		{
			return bytes.data();
		}

		inline void SetByte(uint index, uchar value)
		{
			bytes[index] = value;
		}

		inline void SetByte(uint x, uint y, uchar value)
		{
			bytes[y * width + x] = value;
		}

		inline void SetWidth(uint newWidth)
		{
			width = newWidth;
		}

		inline void SetHeight(uint newHeight)
		{
			height = newHeight;
		}

		inline void SetSize(uint newWidth, uint newHeight)
		{
			width = newWidth;
			height = newHeight;
		}

		inline void SetChannels(uint newChannels)
		{
			channels = newChannels;
		}

		void Reset()
		{
			bytes.assign(GetVolume(), 0);
		}

		void Resize()
		{
			bytes.resize(GetVolume());
		}

		void Read(const std::string& path);
		void Close();

		void Write(const std::string& path);

	private:
		uint width;
		uint height;
		uint channels;
		std::vector<uchar> bytes;
	};
}
