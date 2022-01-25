#pragma once

#include <string>

using std::string;

namespace Anggur
{
	class Image
	{
	public:
		Image();
		Image(const string& path);
		~Image();

		inline uint GetWidth() const
		{
			return _width;
		}
		inline uint GetHeight() const
		{
			return _height;
		}
		inline uint GetChannels() const
		{
			return _channels;
		}
		inline uint8_t* GetData() const
		{
			return _data;
		}

		void Load(const string& path);
		void Unload();

	private:
		uint _width;
		uint _height;
		uint _channels;
		uint8_t* _data;
	};
}

