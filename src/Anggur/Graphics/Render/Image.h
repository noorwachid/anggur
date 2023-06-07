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

		inline unsigned int GetWidth() const
		{
			return width;
		}

		inline unsigned int GetHeight() const
		{
			return height;
		}

		inline unsigned int GetChannels() const
		{
			return channels;
		}

		inline unsigned int GetVolume() const
		{
			return width * height * channels;
		}

		inline const std::vector<unsigned char>& GetBytes() const
		{
			return bytes;
		}

		inline std::vector<unsigned char>& GetBytes()
		{
			return bytes;
		}

		inline unsigned char* ToPointer()
		{
			return bytes.data();
		}

		inline void SetByte(unsigned int index, unsigned char value)
		{
			bytes[index] = value;
		}

		inline void SetByte(unsigned int x, unsigned int y, unsigned char value)
		{
			bytes[y * width + x] = value;
		}

		inline void SetWidth(unsigned int newWidth)
		{
			width = newWidth;
		}

		inline void SetHeight(unsigned int newHeight)
		{
			height = newHeight;
		}

		inline void SetSize(unsigned int newWidth, unsigned int newHeight)
		{
			width = newWidth;
			height = newHeight;
		}

		inline void SetChannels(unsigned int newChannels)
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
		unsigned int width;
		unsigned int height;
		unsigned int channels;
		std::vector<unsigned char> bytes;
	};
}
