#pragma once

#include "Anggur/IO/File.h"
#include "Texture2D.h"
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
        uint32_t pointerX = 0;
        uint32_t pointerY = 0;

        bool occupied = false;

        Image image;

        GlyphBuffer(uint32_t size)
        {
            image.SetSize(size, size);
            image.SetChannels(1);
            image.Resize();
        }
    };

    struct TextSystem
    {
        Font font;
        uint32_t glyphSamplingSize;
        uint32_t glyphAtlasSize;
        uint32_t glyphMaxHeight = 0;
        std::vector<GlyphBuffer> glyphBuffers;

        void SetFont(const std::string& newPath, uint32_t newGlyphSamplingSize, uint32_t newGlyphAtlasSize)
        {
            font.Load(newPath);
            glyphSamplingSize = newGlyphSamplingSize;
            glyphAtlasSize = newGlyphAtlasSize;

            if (glyphAtlasSize < glyphSamplingSize) {
                throw std::runtime_error("Glyph atlas size must be greater than sampling size");
            }
        }

        void Generate(uint32_t newCodePoint = 'F', uint32_t next = 1)
        {
            if (glyphBuffers.empty() || glyphBuffers.back().occupied) 
                glyphBuffers.push_back(GlyphBuffer(glyphAtlasSize));

            float scale = stbtt_ScaleForPixelHeight(font.context, glyphSamplingSize);
    
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

                int glyphWidth = x2 - x1;
                int glyphHeight = y2 - y1;

                if (glyphMaxHeight < glyphHeight)
                    glyphMaxHeight = glyphHeight;
                
                if (glyphBuffers.back().pointerX + roundf(ax * scale) > glyphBuffers.back().image.GetWidth())
                {
                    glyphBuffers.back().pointerX = 0;
                    glyphBuffers.back().pointerY += glyphMaxHeight;

                    glyphMaxHeight = 0;
                }
                
                int byteOffset = glyphBuffers.back().pointerX + roundf(lsb * scale) + ((glyphBuffers.back().pointerY) * glyphBuffers.back().image.GetWidth());

                if (glyphBuffers.back().pointerY + glyphHeight > glyphBuffers.back().image.GetHeight())
                {
                    glyphBuffers.back().occupied = true; 
                    glyphBuffers.push_back(GlyphBuffer(glyphAtlasSize));
                    byteOffset = glyphBuffers.back().pointerX + roundf(lsb * scale) + ((glyphBuffers.back().pointerY) * glyphBuffers.back().image.GetWidth());
                }

                stbtt_MakeCodepointBitmap(font.context, glyphBuffers.back().image.ToPointer() + byteOffset, glyphWidth, glyphHeight, glyphBuffers.back().image.GetWidth(), scale, scale, codePoint);

                glyphBuffers.back().pointerX += roundf(ax * scale);
                
                int kern = stbtt_GetCodepointKernAdvance(font.context, codePoint, codePoint + 1);
                glyphBuffers.back().pointerX += roundf(kern * scale);
            }
        }

        void GenerateASCII()
        {
            Generate(33, 94);
        }
    };
}