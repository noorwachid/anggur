#pragma once

#include <string>
#include <vector>

namespace Anggur
{
    class Image
    {
      public:
        Image();
        Image(const std::string &path);

        ~Image();

        inline uint32_t getWidth() const
        {
            return width;
        }
        inline uint32_t getHeight() const
        {
            return height;
        }
        inline uint32_t getChannels() const
        {
            return channels;
        }
        inline uint32_t getVolume() const
        {
            return width * height * channels;
        }

        inline const std::vector<uint8_t> &getBytes() const
        {
            return bytes;
        }
        inline std::vector<uint8_t> &getBytes()
        {
            return bytes;
        }

        inline uint8_t *toPointer()
        {
            return bytes.data();
        }

        inline void setByte(uint32_t index, uint8_t value)
        {
            bytes[index] = value;
        }
        inline void setByte(uint32_t x, uint32_t y, uint8_t value)
        {
            bytes[y * width + x] = value;
        }
        inline void setWidth(uint32_t newWidth)
        {
            width = newWidth;
        };
        inline void setHeight(uint32_t newHeight)
        {
            height = newHeight;
        };
        inline void setSize(uint32_t newWidth, uint32_t newHeight)
        {
            width = newWidth;
            height = newHeight;
        };
        inline void setChannels(uint32_t newChannels)
        {
            channels = newChannels;
        };

        void resize()
        {
            bytes.resize(getVolume());
        }

        void load(const std::string &path);
        void unload();

        void save(const std::string &path);

      private:
        uint32_t width;
        uint32_t height;
        uint32_t channels;
        std::vector<uint8_t> bytes;
    };
}
