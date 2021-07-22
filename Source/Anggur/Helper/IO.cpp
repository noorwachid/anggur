#include "IO.hpp"
#include "Log.hpp"

namespace Anggur {

std::string IO::Read(const std::string& path)
{
    std::string wrapper;

    FILE* file;
    char* buffer;
    long length;

    file = fopen(path.c_str(), "r");
    if (file == nullptr)
    {
        Anggur_Log("File.Read :: Cannot open file");
        return wrapper;
    }

    fseek(file, 0L, SEEK_END);
    length = ftell(file);

    if (length == -1)
    {
        Anggur_Log("File.Read :: File is broken");
        return wrapper;
    }

    fseek(file, 0L, SEEK_SET);
    buffer = new char[length + 1];
    fread(buffer, sizeof(char), length, file);
    buffer[length] = '\0';

    if (ferror(file) != 0)
    {
        Anggur_Log("File.Read :: Error while reading file");
        return wrapper;
    }

    fclose(file);
    wrapper = buffer;
    delete[] buffer;

    return wrapper;
}

uchar* IO::Load(const std::string& path, usize& size)
{
    FILE* file = fopen(path.c_str(), "rb");

    if (file)
    {
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (size > 0)
        {
            uchar* buffer = new uchar[size];
            usize read = fread(buffer, sizeof(uchar), size, file);

            if (read != size)
                Anggur_Log("[Core.Io] File only read partially");

            return buffer;
        }
        fclose(file);
        Anggur_Log("[Core.Io] File is empty");
    }

    Anggur_Log("[Core.Io] :: Failed to open file");

    return nullptr;
}

std::string IO::GetFileExtention(const std::string& path)
{
    usize index = path.length();

    for (auto it = path.rbegin(); it != path.rend(); ++it, --index)
        if (*it == '.')
            break;

    if (index == 0 || index == path.length())
        return "";

    return path.substr(index, path.length() - index + 1);
}

}
