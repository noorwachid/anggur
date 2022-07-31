#pragma once

#include "Anggur/IO/File.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

#include "stb_image_write.h"
#include "stb_truetype.h"

struct stbtt_fontinfo;

namespace Anggur
{
    using FontContext = stbtt_fontinfo;

    struct Font
    {
        ~Font()
        {
        }

        void Load(const std::string& path)
        {
            FILE* fontFile = fopen(path.c_str(), "rb");
            fseek(fontFile, 0, SEEK_END);
            size_t byteSize = ftell(fontFile);
            fseek(fontFile, 0, SEEK_SET);
            
            data.resize(byteSize);
            
            fread(data.data(), byteSize, 1, fontFile);
            fclose(fontFile);

            Initialize();
        }

        void Initialize()
        {
            context = new FontContext;

            int result = stbtt_InitFont(context, data.data(), 0);

            if (!result)
                throw std::runtime_error("Failed to initilaze font [" + std::to_string(result) + "]");
        }

        FontContext* context;
        std::vector<uint8_t> data;
    };

    struct Glyph
    {
        uint32_t x;
        uint32_t y;
        uint32_t width;
        uint32_t height;

        uint32_t ascent;
        uint32_t descent;

        size_t bufferIndex = 0;
    };

    struct GlyphBuffer
    {
        uint32_t width = 512;
        uint32_t height = 512;

        uint32_t pointerX = 0;
        uint32_t pointerY = 0;

        bool occupied = false;

        std::vector<uint8_t> data;

        GlyphBuffer()
        {
            data.resize(width * height);
        }

        void Save(const std::string& path)
        {
            stbi_write_png((path + "_" + std::to_string(width) + "x" + std::to_string(height) + ".png").c_str(), width, height, 1, data.data(), width);
        }
    };

    struct TextSystem
    {
        Font font;
        uint32_t glyphSize;
        uint32_t glyphMaxHeight = 0;
        std::vector<GlyphBuffer> glyphBuffers;

        void SetFont(const std::string& newPath, uint32_t newGlyphSize)
        {
            font.Load(newPath);
            glyphSize = newGlyphSize;
        }

        void Generate(uint32_t newCodePoint = 33, uint32_t next = 1)
        {
            if (glyphBuffers.empty() || glyphBuffers.back().occupied) 
                glyphBuffers.push_back(GlyphBuffer());

            /* calculate font scaling */
            float scale = stbtt_ScaleForPixelHeight(font.context, glyphSize);
    
            int ascent, descent, lineGap;
            stbtt_GetFontVMetrics(font.context, &ascent, &descent, &lineGap);

            ascent = roundf(ascent * scale);
            descent = roundf(descent * scale);

            for (uint32_t i = 0; i < next; ++i)
            {
                int codePoint = i + newCodePoint;
                int ax, lsb;
                stbtt_GetCodepointHMetrics(font.context, codePoint, &ax, &lsb);

                int x1, y1, x2, y2;
                stbtt_GetCodepointBitmapBox(font.context, codePoint, scale, scale, &x1, &y1, &x2, &y2);

                x2 = x2 + abs(x1);
                x1 = 0;

                y2 = y2 + abs(y1);
                y1 = 0;

                if (glyphMaxHeight < y2)
                    glyphMaxHeight = y2;
                
                int pointerY = y1;

                if (glyphBuffers.back().pointerX + roundf(ax * scale) > glyphBuffers.back().width)
                {
                    glyphBuffers.back().pointerX = 0;
                    glyphBuffers.back().pointerY += glyphMaxHeight;

                    glyphMaxHeight = 0;
                }
                
                int byteOffset = glyphBuffers.back().pointerX + roundf(lsb * scale) + ((glyphBuffers.back().pointerY + pointerY) * glyphBuffers.back().width);

                if (glyphBuffers.back().pointerY + y2 > glyphBuffers.back().height)
                {
                    glyphBuffers.back().occupied = true; 
                    glyphBuffers.push_back(GlyphBuffer());
                    byteOffset = glyphBuffers.back().pointerX + roundf(lsb * scale) + ((glyphBuffers.back().pointerY + pointerY) * glyphBuffers.back().width);
                }

                stbtt_MakeCodepointBitmap(font.context, glyphBuffers.back().data.data() + byteOffset, x2 - x1, y2 - y1, glyphBuffers.back().width, scale, scale, codePoint);

                glyphBuffers.back().pointerX += roundf(ax * scale);
                
                int kern = stbtt_GetCodepointKernAdvance(font.context, codePoint, codePoint + 1);
                glyphBuffers.back().pointerX += roundf(kern * scale);
            }
        }

        void GenerateASCII()
        {
            Generate(33, 94);
        }

        void Save(const std::string& path)
        {
            for (size_t i = 0; i < glyphBuffers.size(); ++i) 
            {
                glyphBuffers[i].Save(path + "_" + std::to_string(i));
            }
        }
    };
}