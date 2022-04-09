#pragma once

#include "Anggur/Graphic/Texture.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Utility/Enum.h"
#include <unordered_map>

struct stbtt_fontinfo;

namespace Anggur {
	struct Glyph {
		Vector2 size;
		float x;
		float y;
		float w;
		float h;
		float ascent;
	};

	class Font {
	public:
		Font();
		~Font();

		void initialize();
		void load(const std::string& path, int sampleSize = 100);

		Texture& getTexture();
		const Texture& getTexture() const;

		float getKerning(int c0, int c1);
		float getLineHeight();
		int getSampleSize();

	private:
		stbtt_fontinfo* info;
		std::vector<uint8_t> buffer;

		Texture texture;
		std::vector<int> ranges;

	public:
		std::unordered_map<int, Glyph> glyphs;

	private:
		Vector2 normal;
		float lineHeight;
		float invSampleSize;
		int sampleSize;
	};
}