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
			return _width;
		}

		inline unsigned int GetHeight() const
		{
			return _height;
		}

		inline unsigned int GetChannels() const
		{
			return _channels;
		}

		inline unsigned int GetVolume() const
		{
			return _width * _height * _channels;
		}

		inline const std::vector<unsigned char>& GetBytes() const
		{
			return _bytes;
		}

		inline std::vector<unsigned char>& GetBytes()
		{
			return _bytes;
		}

		inline unsigned char* ToPointer()
		{
			return _bytes.data();
		}

		inline void SetByte(unsigned int index, unsigned char value)
		{
			_bytes[index] = value;
		}

		inline void SetByte(unsigned int x, unsigned int y, unsigned char value)
		{
			_bytes[y * _width + x] = value;
		}

		inline void SetWidth(unsigned int width)
		{
			this->_width = width;
		}

		inline void SetHeight(unsigned int height)
		{
			this->_height = height;
		}

		inline void SetSize(unsigned int width, unsigned int height)
		{
			this->_width = width;
			this->_height = height;
		}

		inline void SetChannels(unsigned int channels)
		{
			this->_channels = channels;
		}

		void Reset()
		{
			_bytes.assign(GetVolume(), 0);
		}

		void Resize()
		{
			_bytes.resize(GetVolume());
		}

		void Read(const std::string& path);

		void Close();

		void Write(const std::string& path);

	private:
		unsigned int _width;
		unsigned int _height;
		unsigned int _channels;

		std::vector<unsigned char> _bytes;
	};
}
