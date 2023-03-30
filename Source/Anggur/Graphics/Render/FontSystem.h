#pragma once 

#include "Font.h"

namespace Anggur 
{
	class FontSystem
	{
	public:
		std::unordered_map<std::string, Font*> fonts;
		
		void Load(const std::string& path)
		{
			Font* font = new Font();
			font->SetSample(path, 32, 512);
			font->GenerateASCII();

			std::cout << "Font Loaded: " << font->GetName() << "\n";

			fonts[font->GetName()] = font;
		}
		
		Font* GetFont(const std::string& name)
		{
			if (fonts.count(name) < 1)
				return nullptr;

			return fonts[name];
		}
	};
}
