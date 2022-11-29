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

namespace Anggur {
    using FontContext = stbtt_fontinfo;

    struct Glyph {
        Vector2 offset;
        Vector2 size;

        float ascent;
        float descent;

        size_t bufferIndex = 0;
    };

    struct GlyphBuffer {
        uint32_t pointerX = 0;
        uint32_t pointerY = 0;

        bool occupied = false;
        bool dirty = true;

        Image image;
        std::shared_ptr<Texture2D> texture;

        GlyphBuffer(uint32_t size) {
            image.setSize(size, size);
            image.setChannels(1);
            image.resize();
        }
    };

    struct Font {
        FontContext* context;
        std::vector<uint8_t> data;
        
        uint32_t glyphSamplingSize;
        uint32_t glyphAtlasSize;
        uint32_t glyphMaxHeight = 0;

        std::vector<GlyphBuffer> glyphBuffers;
        std::unordered_map<uint32_t, Glyph> glyphMap;

         void load(const std::string& path) {
            FILE* fontFile = fopen(path.c_str(), "rb");
            fseek(fontFile, 0, SEEK_END);
            size_t byteSize = ftell(fontFile);
            fseek(fontFile, 0, SEEK_SET);
            
            data.resize(byteSize);
            
            fread(data.data(), byteSize, 1, fontFile);
            fclose(fontFile);

            initialize();
        }

        void initialize() {
            context = new FontContext;

            int result = stbtt_InitFont(context, data.data(), 0);

            if (!result)
                throw std::runtime_error("Failed to initilaze font [" + std::to_string(result) + "]");
        }

        void setSample(const std::string& newPath, uint32_t newGlyphSamplingSize, uint32_t newGlyphAtlasSize) {
            load(newPath);
            glyphSamplingSize = newGlyphSamplingSize;
            glyphAtlasSize = newGlyphAtlasSize;

            if (glyphAtlasSize < glyphSamplingSize) {
                throw std::runtime_error("Glyph atlas size must be greater than sampling size");
            }
        }

        void generate(uint32_t newCodePoint = 'F', uint32_t next = 1) {
            if (glyphBuffers.empty() || glyphBuffers.back().occupied) 
                glyphBuffers.push_back(GlyphBuffer(glyphAtlasSize));

            float scale = stbtt_ScaleForPixelHeight(context, glyphSamplingSize);
    
            int ascent, descent, lineGap;
            stbtt_GetFontVMetrics(context, &ascent, &descent, &lineGap);

            ascent = roundf(ascent * scale);
            descent = roundf(descent * scale);

            float pixelDistanceScale = 32.0;  // trades off precision w/ ability to handle *smaller* sizes
            int edgeValue = 206;
            int padding = 6; // not used in shade

            for (uint32_t i = 0; i < next; ++i) {
                int codePoint = i + newCodePoint;
                int glyphWidth;
                int glyphHeight;
                int glyphX;
                int glyphY;

                uint8_t* buffer = stbtt_GetCodepointSDF(context, scale, codePoint, padding, edgeValue, pixelDistanceScale, &glyphWidth, &glyphHeight, &glyphX, &glyphY);

                if (glyphMaxHeight < glyphHeight)
                    glyphMaxHeight = glyphHeight;
                
                if (glyphBuffers.back().pointerX + glyphWidth > glyphBuffers.back().image.getWidth()) {
                    glyphBuffers.back().pointerX = 0;
                    glyphBuffers.back().pointerY += glyphMaxHeight;

                    glyphMaxHeight = 0;
                }

                if (glyphBuffers.back().pointerY + glyphHeight > glyphBuffers.back().image.getHeight()) {
                    glyphBuffers.back().occupied = true; 

                    if (glyphBuffers.back().dirty) {
                        glyphBuffers.back().texture = std::make_shared<Texture2D>(glyphBuffers.back().image);
                        glyphBuffers.back().dirty = false;
                    }

                    glyphBuffers.push_back(GlyphBuffer(glyphAtlasSize));
                }

                float normal = 1.0f / glyphAtlasSize;

                Glyph glyph;
                glyph.offset.set((glyphBuffers.back().pointerX) * normal, glyphBuffers.back().pointerY * normal);
                glyph.size.set(glyphWidth * normal, glyphHeight * normal);
                glyph.ascent = (ascent + glyphY) * normal;
                glyph.descent = -descent * normal;
                glyph.bufferIndex = glyphBuffers.size() - 1;
                glyphMap[codePoint] = glyph;

                for (int x = 0; x < glyphWidth; ++x) 
                    for (int y = 0; y < glyphHeight; ++y)
                        glyphBuffers.back().image.setByte(x + glyphBuffers.back().pointerX, y + glyphBuffers.back().pointerY, buffer[y * glyphWidth + x]);

                glyphBuffers.back().pointerX += glyphWidth;
            }

            glyphBuffers.back().texture = std::make_shared<Texture2D>(glyphBuffers.back().image);
        }

        void generateASCII() {
            generate(33, 94);
        }
    };
}