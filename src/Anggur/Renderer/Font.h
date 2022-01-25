#pragma once

#include "Texture.h"
#include <Anggur/Helper/Enum.h>
#include <Anggur/Math/Vector.h>
#include <unordered_map>

struct stbtt_fontinfo;

namespace Anggur
{
	struct Glyph
	{
		Vector size;
		float x;
		float y;
		float w;
		float h;
		float ascent;
	};

	class Font
	{
	public:
		Font();
		~Font();

		void Initialize();
		void Load(const string& path, int sampleSize = 100);

		Texture& GetTexture();
		const Texture& GetTexture() const;

		float GetKerning(int c0, int c1);
		float GetLineHeight();
		int GetSampleSize();

	private:
		stbtt_fontinfo* _info;
		std::vector<uint8_t> _buffer;

		Texture _texture;
		std::vector<int> _ranges;

	public:
		std::unordered_map<int, Glyph> glyphs;

	private:
		Vector _normal;
		float _lineHeight;
		float _invSampleSize;
		int _sampleSize;
	};
}