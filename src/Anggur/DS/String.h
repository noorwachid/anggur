#pragma once

#include <array>
#include <vector>
#include <bitset>
#include <iostream>

namespace Anggur
{
    class String
    {
    public:
        String(const char *text)
        {
            size_t i = 0;
            std::string str = text;
            while (i < str.size())
            {
                std::cout << "Char: " << text[i] << "\n";
                codepoints.push_back(CharArrayToCodepoint(text, i));
                
                std::cout << "CharArray Size: " << (int)(unsigned char)text[i] << " " << GetCharArrayChuckSize(text[i]) << "\n";
                i += charArraySizeMap[text[i]];
            }
        }

        String(int newCodepoint)
        {
            codepoints.clear();
            codepoints.push_back(newCodepoint);
        }

        String(const std::vector<int> &newCodepoints)
        {
            codepoints = newCodepoints;
        }

        size_t GetSize() const
        {
            return codepoints.size();
        }

        int operator[](size_t index) const
        {
            return codepoints[index];
        }

        int operator[](size_t index)
        {
            return codepoints[index];
        }

        bool operator==(const String &other)
        {
            if (GetSize() != other.GetSize())
                return false;

            for (size_t i = 0; i < GetSize(); ++i)
            {
                if (codepoints[i] != other[i])
                    return false;
            }

            return true;
        }

        size_t GetCharArrayChuckSize(char c)
        {
            return charArraySizeMap[static_cast<unsigned char>(c)];
        }

        int CharArrayToCodepoint(const char* c, size_t offset = 0)
        {
            if (c[offset] == 0)
                return 0;

            int size = GetCharArrayChuckSize(c[offset]);
            char mask = charArrayMask[size - 1];
            int result = c[offset] & mask;
            for (int i = 1; i < size; ++i)
            {
                result <<= 6;
                result |= c[offset + i] & 0x3f;
            }

            return result;
        }

        int CodepointToCharArray(char *out, uint32_t c)
        {
            int len = 0;
            int first;
            int i;

            if (c < 0x80)
            {
                first = 0;
                len = 1;
            }
            else if (c < 0x800)
            {
                first = 0xc0;
                len = 2;
            }
            else if (c < 0x10000)
            {
                first = 0xe0;
                len = 3;
            }
            else if (c < 0x200000)
            {
                first = 0xf0;
                len = 4;
            }
            else if (c < 0x4000000)
            {
                first = 0xf8;
                len = 5;
            }
            else
            {
                first = 0xfc;
                len = 6;
            }

            for (i = len - 1; i > 0; --i)
            {
                out[i] = (c & 0x3f) | 0x80;
                c >>= 6;
            }
            out[0] = c | first;

            return len;
        }

    private:
        static std::array<size_t, 256> charArraySizeMap;
        static std::array<char, 6> charArrayMask;

    private:
        std::vector<int> codepoints;
    };
}
