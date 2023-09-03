#pragma once

#include <vector>

namespace Anggur
{
	enum class SamplerFilter
	{
		Nearest,
		Linear
	};

	class TextureSpecification
	{
	public:
		static int GetMaxSlot();
	};

	class Texture
	{
	public:
		Texture();
		Texture(
			const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height, unsigned int channels = 8,
			SamplerFilter filter = SamplerFilter::Linear
		);

		~Texture();

		unsigned int GetID() const
		{
			return _id;
		}

		unsigned int GetWidth() const
		{
			return _width;
		}

		unsigned int GetHeight() const
		{
			return _height;
		}

		unsigned int GetChannels() const
		{
			return _channels;
		}

		void Read(
			const std::vector<unsigned char>& bytes, unsigned int width, unsigned int height, unsigned int channels = 8,
			SamplerFilter filter = SamplerFilter::Linear
		);

		void Close();

		void Bind(unsigned int slot = 0);

		friend bool operator==(const Texture& a, const Texture& b);

	private:
		unsigned int _id;
		unsigned int _width;
		unsigned int _height;
		unsigned int _channels;
	};
}
