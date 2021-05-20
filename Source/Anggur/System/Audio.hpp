#pragma once

#include <string>

struct Mix_Chunk;
struct _Mix_Music;

namespace Anggur {

class Audio
{
public:
    static void SetBuffer(int frequency = 44100, int bufferSize = 128);
    static void SetPollSize(int size);

    static void Initialize();
    static void Terminate();

private:
    static int mFrequency;
    static int mBufferSize;
    static int mPollSize;
};

class SoundFx
{
public:
    SoundFx();
    ~SoundFx();

    void Load(const std::string& path);
    void Unload();
    void Play();
    void Pause();

private:
    Mix_Chunk* mChunkData;
};

class Music
{
public:
    Music();
    ~Music();

    void Load(const std::string& path);
    void Unload();
    void Play();
    void Pause();

private:
    _Mix_Music* mMusicData;
};

}
